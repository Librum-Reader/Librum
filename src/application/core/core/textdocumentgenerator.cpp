/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "textdocumentgenerator.h"
#include "textdocumentgenerator_p.h"

#include <QFile>
#include <QFontDatabase>
#include <QImage>
#include <QMutex>
#include <QPainter>
#include <QPrinter>
#include <QStack>
#include <QTextDocumentWriter>
#include <QTextStream>
#include <QVector>

#include "action.h"
#include "annotations.h"
#include "document_p.h"
#include "page.h"
#include "textpage.h"

#include <cmath>

using namespace Okular;

/**
 * Generic Converter Implementation
 */
TextDocumentConverter::TextDocumentConverter()
    : QObject(nullptr)
    , d_ptr(new TextDocumentConverterPrivate)
{
}

TextDocumentConverter::~TextDocumentConverter()
{
    delete d_ptr;
}

QTextDocument *TextDocumentConverter::convert(const QString &)
{
    return nullptr;
}

Document::OpenResult TextDocumentConverter::convertWithPassword(const QString &fileName, const QString &)
{
    QTextDocument *doc = convert(fileName);
    setDocument(doc);
    return doc != nullptr ? Document::OpenSuccess : Document::OpenError;
}

QTextDocument *TextDocumentConverter::document()
{
    return d_ptr->mDocument;
}

void TextDocumentConverter::setDocument(QTextDocument *document)
{
    d_ptr->mDocument = document;
}

DocumentViewport TextDocumentConverter::calculateViewport(QTextDocument *document, const QTextBlock &block)
{
    return TextDocumentUtils::calculateViewport(document, block);
}

TextDocumentGenerator *TextDocumentConverter::generator() const
{
    return d_ptr->mParent ? d_ptr->mParent->q_func() : nullptr;
}

/**
 * Generic Generator Implementation
 */
Okular::TextPage *TextDocumentGeneratorPrivate::createTextPage(int pageNumber) const
{
#ifdef OKULAR_TEXTDOCUMENT_THREADED_RENDERING
    Q_Q(const TextDocumentGenerator);
#endif

    Okular::TextPage *textPage = new Okular::TextPage;

    int start, end;

#ifdef OKULAR_TEXTDOCUMENT_THREADED_RENDERING
    q->userMutex()->lock();
#endif
    TextDocumentUtils::calculatePositions(mDocument, pageNumber, start, end);

    {
        QTextCursor cursor(mDocument);
        for (int i = start; i < end - 1; ++i) {
            cursor.setPosition(i);
            cursor.setPosition(i + 1, QTextCursor::KeepAnchor);

            QString text = cursor.selectedText();
            if (text.length() == 1) {
                QRectF rect;
                TextDocumentUtils::calculateBoundingRect(mDocument, i, i + 1, rect, pageNumber);
                if (pageNumber == -1) {
                    text = QStringLiteral("\n");
                }

                textPage->append(text, new Okular::NormalizedRect(rect.left(), rect.top(), rect.right(), rect.bottom()));
            }
        }
    }
#ifdef OKULAR_TEXTDOCUMENT_THREADED_RENDERING
    q->userMutex()->unlock();
#endif

    return textPage;
}

void TextDocumentGeneratorPrivate::addAction(Action *action, int cursorBegin, int cursorEnd)
{
    if (!action) {
        return;
    }

    LinkPosition position;
    position.link = action;
    position.startPosition = cursorBegin;
    position.endPosition = cursorEnd;

    mLinkPositions.append(position);
}

void TextDocumentGeneratorPrivate::addAnnotation(Annotation *annotation, int cursorBegin, int cursorEnd)
{
    if (!annotation) {
        return;
    }

    annotation->setFlags(annotation->flags() | Okular::Annotation::External);

    AnnotationPosition position;
    position.annotation = annotation;
    position.startPosition = cursorBegin;
    position.endPosition = cursorEnd;

    mAnnotationPositions.append(position);
}

void TextDocumentGeneratorPrivate::addTitle(int level, const QString &title, const QTextBlock &block)
{
    TitlePosition position;
    position.level = level;
    position.title = title;
    position.block = block;

    mTitlePositions.append(position);
}

void TextDocumentGeneratorPrivate::addMetaData(DocumentInfo::Key key, const QString &value)
{
    mDocumentInfo.set(key, value);
}

QList<TextDocumentGeneratorPrivate::LinkInfo> TextDocumentGeneratorPrivate::generateLinkInfos() const
{
    QList<LinkInfo> result;

    for (int i = 0; i < mLinkPositions.count(); ++i) {
        const LinkPosition &linkPosition = mLinkPositions[i];

        const QVector<QRectF> rects = TextDocumentUtils::calculateBoundingRects(mDocument, linkPosition.startPosition, linkPosition.endPosition);

        for (int i = 0; i < rects.count(); ++i) {
            const QRectF &rect = rects[i];

            LinkInfo info;
            info.link = linkPosition.link;
            info.ownsLink = i == 0;
            info.page = std::floor(rect.y());
            info.boundingRect = QRectF(rect.x(), rect.y() - info.page, rect.width(), rect.height());
            result.append(info);
        }
    }

    return result;
}

QList<TextDocumentGeneratorPrivate::AnnotationInfo> TextDocumentGeneratorPrivate::generateAnnotationInfos() const
{
    QList<AnnotationInfo> result;

    for (int i = 0; i < mAnnotationPositions.count(); ++i) {
        const AnnotationPosition &annotationPosition = mAnnotationPositions[i];

        AnnotationInfo info;
        info.annotation = annotationPosition.annotation;

        TextDocumentUtils::calculateBoundingRect(mDocument, annotationPosition.startPosition, annotationPosition.endPosition, info.boundingRect, info.page);

        if (info.page >= 0) {
            result.append(info);
        }
    }

    return result;
}

void TextDocumentGeneratorPrivate::generateTitleInfos()
{
    QStack<QPair<int, QDomNode>> parentNodeStack;

    QDomNode parentNode = mDocumentSynopsis;

    parentNodeStack.push(qMakePair(0, parentNode));

    for (int i = 0; i < mTitlePositions.count(); ++i) {
        const TitlePosition &position = mTitlePositions[i];

        Okular::DocumentViewport viewport = TextDocumentUtils::calculateViewport(mDocument, position.block);

        QDomElement item = mDocumentSynopsis.createElement(position.title);
        item.setAttribute(QStringLiteral("Viewport"), viewport.toString());

        int headingLevel = position.level;

        // we need a parent, which has to be at a higher heading level than this heading level
        // so we just work through the stack
        while (!parentNodeStack.isEmpty()) {
            int parentLevel = parentNodeStack.top().first;
            if (parentLevel < headingLevel) {
                // this is OK as a parent
                parentNode = parentNodeStack.top().second;
                break;
            } else {
                // we'll need to be further into the stack
                parentNodeStack.pop();
            }
        }
        parentNode.appendChild(item);
        parentNodeStack.push(qMakePair(headingLevel, QDomNode(item)));
    }
}

void TextDocumentGeneratorPrivate::initializeGenerator()
{
    Q_Q(TextDocumentGenerator);

    mConverter->d_ptr->mParent = q->d_func();

    if (mGeneralSettings) {
        mFont = mGeneralSettings->font();
    }

    q->setFeature(Generator::TextExtraction);
    q->setFeature(Generator::PrintNative);
    q->setFeature(Generator::PrintToFile);
#ifdef OKULAR_TEXTDOCUMENT_THREADED_RENDERING
    q->setFeature(Generator::Threaded);
#endif

    QObject::connect(mConverter, &TextDocumentConverter::addAction, q, [this](Action *a, int cb, int ce) { addAction(a, cb, ce); });
    QObject::connect(mConverter, &TextDocumentConverter::addAnnotation, q, [this](Annotation *a, int cb, int ce) { addAnnotation(a, cb, ce); });
    QObject::connect(mConverter, &TextDocumentConverter::addTitle, q, [this](int l, const QString &t, const QTextBlock &b) { addTitle(l, t, b); });
    QObject::connect(mConverter, &TextDocumentConverter::addMetaData, q, [this](DocumentInfo::Key k, const QString &v) { addMetaData(k, v); });

    QObject::connect(mConverter, &TextDocumentConverter::error, q, &Generator::error);
    QObject::connect(mConverter, &TextDocumentConverter::warning, q, &Generator::warning);
    QObject::connect(mConverter, &TextDocumentConverter::notice, q, &Generator::notice);
}

TextDocumentGenerator::TextDocumentGenerator(TextDocumentConverter *converter, const QString &configName, QObject *parent, const QVariantList &args)
    : Okular::Generator(*new TextDocumentGeneratorPrivate(converter), parent, args)
{
    Q_D(TextDocumentGenerator);
    d->mGeneralSettings = new TextDocumentSettings(configName, this);

    d->initializeGenerator();
}

TextDocumentGenerator::~TextDocumentGenerator()
{
}

Document::OpenResult TextDocumentGenerator::loadDocumentWithPassword(const QString &fileName, QVector<Okular::Page *> &pagesVector, const QString &password)
{
    Q_D(TextDocumentGenerator);
    const Document::OpenResult openResult = d->mConverter->convertWithPassword(fileName, password);

    if (openResult != Document::OpenSuccess) {
        d->mDocument = nullptr;

        // loading failed, cleanup all the stuff eventually gathered from the converter
        d->mTitlePositions.clear();
        for (const TextDocumentGeneratorPrivate::LinkPosition &linkPos : qAsConst(d->mLinkPositions)) {
            delete linkPos.link;
        }
        d->mLinkPositions.clear();
        for (const TextDocumentGeneratorPrivate::AnnotationPosition &annPos : qAsConst(d->mAnnotationPositions)) {
            delete annPos.annotation;
        }
        d->mAnnotationPositions.clear();

        return openResult;
    }
    d->mDocument = d->mConverter->document();

    d->generateTitleInfos();
    const QList<TextDocumentGeneratorPrivate::LinkInfo> linkInfos = d->generateLinkInfos();
    const QList<TextDocumentGeneratorPrivate::AnnotationInfo> annotationInfos = d->generateAnnotationInfos();

    pagesVector.resize(d->mDocument->pageCount());

    const QSize size = d->mDocument->pageSize().toSize();

    QVector<QList<Okular::ObjectRect *>> objects(d->mDocument->pageCount());
    for (const TextDocumentGeneratorPrivate::LinkInfo &info : linkInfos) {
        // in case that the converter report bogus link info data, do not assert here
        if (info.page < 0 || info.page >= objects.count()) {
            continue;
        }

        const QRectF rect = info.boundingRect;
        if (info.ownsLink) {
            objects[info.page].append(new Okular::ObjectRect(rect.left(), rect.top(), rect.right(), rect.bottom(), false, Okular::ObjectRect::Action, info.link));
        } else {
            objects[info.page].append(new Okular::NonOwningObjectRect(rect.left(), rect.top(), rect.right(), rect.bottom(), false, Okular::ObjectRect::Action, info.link));
        }
    }

    QVector<QList<Okular::Annotation *>> annots(d->mDocument->pageCount());
    for (const TextDocumentGeneratorPrivate::AnnotationInfo &info : annotationInfos) {
        annots[info.page].append(info.annotation);
    }

    for (int i = 0; i < d->mDocument->pageCount(); ++i) {
        Okular::Page *page = new Okular::Page(i, size.width(), size.height(), Okular::Rotation0);
        pagesVector[i] = page;

        if (!objects.at(i).isEmpty()) {
            page->setObjectRects(objects.at(i));
        }
        QList<Okular::Annotation *>::ConstIterator annIt = annots.at(i).begin(), annEnd = annots.at(i).end();
        for (; annIt != annEnd; ++annIt) {
            page->addAnnotation(*annIt);
        }
    }

    return openResult;
}

bool TextDocumentGenerator::doCloseDocument()
{
    Q_D(TextDocumentGenerator);
    delete d->mDocument;
    d->mDocument = nullptr;

    d->mTitlePositions.clear();
    d->mLinkPositions.clear();
    d->mAnnotationPositions.clear();
    // do not use clear() for the following two, otherwise they change type
    d->mDocumentInfo = Okular::DocumentInfo();
    d->mDocumentSynopsis = Okular::DocumentSynopsis();

    return true;
}

bool TextDocumentGenerator::canGeneratePixmap() const
{
    return Generator::canGeneratePixmap();
}

void TextDocumentGenerator::generatePixmap(Okular::PixmapRequest *request)
{
    Generator::generatePixmap(request);
}

QImage TextDocumentGeneratorPrivate::image(PixmapRequest *request)
{
    if (!mDocument) {
        return QImage();
    }

#ifdef OKULAR_TEXTDOCUMENT_THREADED_RENDERING
    Q_Q(TextDocumentGenerator);
#endif

    QImage image(request->width(), request->height(), QImage::Format_ARGB32);
    image.fill(Qt::white);

    QPainter p;
    p.begin(&image);

    qreal width = request->width();
    qreal height = request->height();

    const QSize size = mDocument->pageSize().toSize();

    p.scale(width / (qreal)size.width(), height / (qreal)size.height());

    QRect rect;
    rect = QRect(0, request->pageNumber() * size.height(), size.width(), size.height());
    p.translate(QPoint(0, request->pageNumber() * size.height() * -1));
    p.setClipRect(rect);
#ifdef OKULAR_TEXTDOCUMENT_THREADED_RENDERING
    q->userMutex()->lock();
#endif
    QAbstractTextDocumentLayout::PaintContext context;
    context.palette.setColor(QPalette::Text, Qt::black);
    //  FIXME Fix Qt, this doesn't work, we have horrible hacks
    //        in the generators that return html, remove that code
    //        if Qt ever gets fixed
    //     context.palette.setColor( QPalette::Link, Qt::blue );
    context.clip = rect;
    mDocument->setDefaultFont(mFont);
    mDocument->documentLayout()->draw(&p, context);
#ifdef OKULAR_TEXTDOCUMENT_THREADED_RENDERING
    q->userMutex()->unlock();
#endif
    p.end();

    return image;
}

Okular::TextPage *TextDocumentGenerator::textPage(Okular::TextRequest *request)
{
    Q_D(TextDocumentGenerator);
    return d->createTextPage(request->page()->number());
}

Document::PrintError TextDocumentGenerator::print(QPrinter &printer)
{
    Q_D(TextDocumentGenerator);
    if (!d->mDocument) {
        return Document::UnknownPrintError;
    }

    d->mDocument->print(&printer);

    return Document::NoPrintError;
}

Okular::DocumentInfo TextDocumentGenerator::generateDocumentInfo(const QSet<DocumentInfo::Key> & /*keys*/) const
{
    Q_D(const TextDocumentGenerator);
    return d->mDocumentInfo;
}

const Okular::DocumentSynopsis *TextDocumentGenerator::generateDocumentSynopsis()
{
    Q_D(TextDocumentGenerator);
    if (!d->mDocumentSynopsis.hasChildNodes()) {
        return nullptr;
    } else {
        return &d->mDocumentSynopsis;
    }
}

QVariant TextDocumentGeneratorPrivate::metaData(const QString &key, const QVariant &option) const
{
    Q_UNUSED(option)
    if (key == QLatin1String("DocumentTitle")) {
        return mDocumentInfo.get(DocumentInfo::Title);
    }
    return QVariant();
}

Okular::ExportFormat::List TextDocumentGenerator::exportFormats() const
{
    static Okular::ExportFormat::List formats;
    if (formats.isEmpty()) {
        formats.append(Okular::ExportFormat::standardFormat(Okular::ExportFormat::PlainText));
        formats.append(Okular::ExportFormat::standardFormat(Okular::ExportFormat::PDF));
        if (QTextDocumentWriter::supportedDocumentFormats().contains("ODF")) {
            formats.append(Okular::ExportFormat::standardFormat(Okular::ExportFormat::OpenDocumentText));
        }
        if (QTextDocumentWriter::supportedDocumentFormats().contains("HTML")) {
            formats.append(Okular::ExportFormat::standardFormat(Okular::ExportFormat::HTML));
        }
    }

    return formats;
}

bool TextDocumentGenerator::exportTo(const QString &fileName, const Okular::ExportFormat &format)
{
    Q_D(TextDocumentGenerator);
    if (!d->mDocument) {
        return false;
    }

    if (format.mimeType().name() == QLatin1String("application/pdf")) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            return false;
        }

        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        d->mDocument->print(&printer);

        return true;
    } else if (format.mimeType().name() == QLatin1String("text/plain")) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            return false;
        }

        QTextStream out(&file);
        out << d->mDocument->toPlainText();

        return true;
    } else if (format.mimeType().name() == QLatin1String("application/vnd.oasis.opendocument.text")) {
        QTextDocumentWriter odfWriter(fileName, "odf");

        return odfWriter.write(d->mDocument);
    } else if (format.mimeType().name() == QLatin1String("text/html")) {
        QTextDocumentWriter odfWriter(fileName, "html");

        return odfWriter.write(d->mDocument);
    }
    return false;
}

bool TextDocumentGenerator::reparseConfig()
{
    Q_D(TextDocumentGenerator);
    const QFont newFont = d->mGeneralSettings->font();

    if (newFont != d->mFont) {
        d->mFont = newFont;
        return true;
    }

    return false;
}

void TextDocumentGenerator::addPages(KConfigDialog * /*dlg*/)
{
    qCWarning(OkularCoreDebug) << "You forgot to reimplement addPages in your TextDocumentGenerator";
    return;
}

TextDocumentSettings *TextDocumentGenerator::generalSettings()
{
    Q_D(TextDocumentGenerator);

    return d->mGeneralSettings;
}

TextDocumentConverter *TextDocumentGenerator::converter()
{
    Q_D(TextDocumentGenerator);

    return d->mConverter;
}

void TextDocumentGenerator::setTextDocument(QTextDocument *textDocument)
{
    Q_D(TextDocumentGenerator);

    d->mDocument = textDocument;

    for (Page *p : qAsConst(d->m_document->m_pagesVector)) {
        p->setTextPage(nullptr);
    }
}
