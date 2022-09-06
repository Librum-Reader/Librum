/*
    SPDX-FileCopyrightText: 2017 Julian Wolff <wolff@julianwolff.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "converter.h"

#include <KLocalizedString>

#include <QTextCursor>
#include <QTextDocument>
#include <QTextFrame>
#include <QTextStream>

#include <core/action.h>

#include "debug_md.h"

extern "C" {
#include <mkdio.h>
}

// older versions of discount might not have these flags.
// defining them to 0 allows us to convert without them
#ifndef MKD_FENCEDCODE
#define MKD_FENCEDCODE 0
#endif

#ifndef MKD_GITHUBTAGS
#define MKD_GITHUBTAGS 0
#endif

#ifndef MKD_AUTOLINK
#define MKD_AUTOLINK 0
#endif

#define PAGE_WIDTH 980
#define PAGE_HEIGHT 1307
#define PAGE_MARGIN 45
#define CONTENT_WIDTH (PAGE_WIDTH - 2 * PAGE_MARGIN)

using namespace Markdown;

Converter::Converter()
    : m_markdownFile(nullptr)
    , m_isFancyPantsEnabled(true)
{
}

Converter::~Converter()
{
    if (m_markdownFile) {
        fclose(m_markdownFile);
    }
}

QTextDocument *Converter::convert(const QString &fileName)
{
    m_markdownFile = fopen(fileName.toLocal8Bit().constData(), "rb");
    if (!m_markdownFile) {
        Q_EMIT error(i18n("Failed to open the document"), -1);
        return nullptr;
    }

    m_fileDir = QDir(fileName.left(fileName.lastIndexOf(QLatin1Char('/'))));

    QTextDocument *doc = convertOpenFile();
    QHash<QString, QTextFragment> internalLinks;
    QHash<QString, QTextBlock> documentAnchors;
    extractLinks(doc->rootFrame(), internalLinks, documentAnchors);

    for (auto linkIt = internalLinks.constBegin(); linkIt != internalLinks.constEnd(); ++linkIt) {
        auto anchorIt = documentAnchors.constFind(linkIt.key());
        if (anchorIt != documentAnchors.constEnd()) {
            const Okular::DocumentViewport viewport = calculateViewport(doc, anchorIt.value());
            Okular::GotoAction *action = new Okular::GotoAction(QString(), viewport);
            Q_EMIT addAction(action, linkIt.value().position(), linkIt.value().position() + linkIt.value().length());
        } else {
            qDebug() << "Could not find destination for" << linkIt.key();
        }
    }

    return doc;
}

void Converter::convertAgain()
{
    setDocument(convertOpenFile());
}

QTextDocument *Converter::convertOpenFile()
{
    rewind(m_markdownFile);

    MMIOT *markdownHandle = mkd_in(m_markdownFile, 0);

    int flags = MKD_FENCEDCODE | MKD_GITHUBTAGS | MKD_AUTOLINK | MKD_TOC | MKD_IDANCHOR;
    if (!m_isFancyPantsEnabled) {
        flags |= MKD_NOPANTS;
    }
    if (!mkd_compile(markdownHandle, flags)) {
        Q_EMIT error(i18n("Failed to compile the Markdown document."), -1);
        return nullptr;
    }

    char *htmlDocument;
    const int size = mkd_document(markdownHandle, &htmlDocument);

    const QString html = QString::fromUtf8(htmlDocument, size);

    QTextDocument *textDocument = new QTextDocument;
    textDocument->setPageSize(QSizeF(PAGE_WIDTH, PAGE_HEIGHT));
    textDocument->setHtml(html);
    if (generator()) {
        textDocument->setDefaultFont(generator()->generalSettings()->font());
    }

    mkd_cleanup(markdownHandle);

    QTextFrameFormat frameFormat;
    frameFormat.setMargin(PAGE_MARGIN);

    QTextFrame *rootFrame = textDocument->rootFrame();
    rootFrame->setFrameFormat(frameFormat);

    convertImages(rootFrame, m_fileDir, textDocument);

    return textDocument;
}

void Converter::extractLinks(QTextFrame *parent, QHash<QString, QTextFragment> &internalLinks, QHash<QString, QTextBlock> &documentAnchors)
{
    for (QTextFrame::iterator it = parent->begin(); !it.atEnd(); ++it) {
        QTextFrame *textFrame = it.currentFrame();
        const QTextBlock textBlock = it.currentBlock();

        if (textFrame) {
            extractLinks(textFrame, internalLinks, documentAnchors);
        } else if (textBlock.isValid()) {
            extractLinks(textBlock, internalLinks, documentAnchors);
        }
    }
}

void Converter::extractLinks(const QTextBlock &parent, QHash<QString, QTextFragment> &internalLinks, QHash<QString, QTextBlock> &documentAnchors)
{
    for (QTextBlock::iterator it = parent.begin(); !it.atEnd(); ++it) {
        const QTextFragment textFragment = it.fragment();
        if (textFragment.isValid()) {
            const QTextCharFormat textCharFormat = textFragment.charFormat();
            if (textCharFormat.isAnchor()) {
                const QString href = textCharFormat.anchorHref();
                if (href.startsWith(QLatin1Char('#'))) { // It's an internal link, store it and we'll resolve it at the end
                    internalLinks.insert(href.mid(1), textFragment);
                } else {
                    Okular::BrowseAction *action = new Okular::BrowseAction(QUrl(textCharFormat.anchorHref()));
                    Q_EMIT addAction(action, textFragment.position(), textFragment.position() + textFragment.length());
                }

                const QStringList anchorNames = textCharFormat.anchorNames();
                for (const QString &anchorName : anchorNames) {
                    documentAnchors.insert(anchorName, parent);
                }
            }
        }
    }
}

void Converter::convertImages(QTextFrame *parent, const QDir &dir, QTextDocument *textDocument)
{
    for (QTextFrame::iterator it = parent->begin(); !it.atEnd(); ++it) {
        QTextFrame *textFrame = it.currentFrame();
        const QTextBlock textBlock = it.currentBlock();

        if (textFrame) {
            convertImages(textFrame, dir, textDocument);
        } else if (textBlock.isValid()) {
            convertImages(textBlock, dir, textDocument);
        }
    }
}

void Converter::convertImages(const QTextBlock &parent, const QDir &dir, QTextDocument *textDocument)
{
    for (QTextBlock::iterator it = parent.begin(); !it.atEnd(); ++it) {
        const QTextFragment textFragment = it.fragment();
        if (textFragment.isValid()) {
            const QTextCharFormat textCharFormat = textFragment.charFormat();
            if (textCharFormat.isImageFormat()) {
                QTextImageFormat format;

                const qreal specifiedHeight = textCharFormat.toImageFormat().height();
                const qreal specifiedWidth = textCharFormat.toImageFormat().width();

                QTextCursor cursor(textDocument);
                cursor.setPosition(textFragment.position(), QTextCursor::MoveAnchor);
                cursor.setPosition(textFragment.position() + textFragment.length(), QTextCursor::KeepAnchor);

                const QString imageFilePath = QDir::cleanPath(dir.absoluteFilePath(QUrl::fromPercentEncoding(textCharFormat.toImageFormat().name().toUtf8())));

                if (QFile::exists(imageFilePath)) {
                    cursor.removeSelectedText();
                    format.setName(imageFilePath);
                    const QImage img = QImage(format.name());

                    setImageSize(format, specifiedWidth, specifiedHeight, img.width(), img.height());

                    cursor.insertImage(format);
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
                } else if ((!textCharFormat.toImageFormat().property(QTextFormat::ImageAltText).toString().isEmpty())) {
                    cursor.insertText(textCharFormat.toImageFormat().property(QTextFormat::ImageAltText).toString());
#endif
                }
            }
        }
    }
}

void Converter::setImageSize(QTextImageFormat &format, const qreal specifiedWidth, const qreal specifiedHeight, const qreal originalWidth, const qreal originalHeight)
{
    qreal width = 0;
    qreal height = 0;

    const bool hasSpecifiedSize = specifiedHeight > 0 || specifiedWidth > 0;
    if (hasSpecifiedSize) {
        width = specifiedWidth;
        height = specifiedHeight;
        if (width == 0 && originalHeight > 0) {
            width = originalWidth * height / originalHeight;
        } else if (height == 0 && originalWidth > 0) {
            height = originalHeight * width / originalWidth;
        }
    } else {
        width = originalWidth;
        height = originalHeight;
    }

    if (width > CONTENT_WIDTH) {
        height = height * CONTENT_WIDTH / width;
        width = CONTENT_WIDTH;
    }
    format.setWidth(width);
    format.setHeight(height);
}
