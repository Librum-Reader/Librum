/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_TEXTDOCUMENTGENERATOR_P_H_
#define _OKULAR_TEXTDOCUMENTGENERATOR_P_H_

#include <QAbstractTextDocumentLayout>
#include <QTextBlock>
#include <QTextDocument>
#include "action.h"
#include "debug_p.h"
#include "document.h"
#include "generator_p.h"
#include "textdocumentgenerator.h"

namespace Okular
{
namespace TextDocumentUtils
{
static void calculateBoundingRect(QTextDocument* document, int startPosition,
                                  int endPosition, QRectF& rect, int& page)
{
    const QSizeF pageSize = document->pageSize();

    const QTextBlock startBlock = document->findBlock(startPosition);
    const QRectF startBoundingRect =
        document->documentLayout()->blockBoundingRect(startBlock);

    const QTextBlock endBlock = document->findBlock(endPosition);
    const QRectF endBoundingRect =
        document->documentLayout()->blockBoundingRect(endBlock);

    const QTextLayout* startLayout = startBlock.layout();
    const QTextLayout* endLayout = endBlock.layout();
    if(!startLayout || !endLayout)
    {
        qCWarning(OkularCoreDebug)
            << "Start or end layout not found" << startLayout << endLayout;
        page = -1;
        return;
    }

    const int startPos = startPosition - startBlock.position();
    const int endPos = endPosition - endBlock.position();
    const QTextLine startLine = startLayout->lineForTextPosition(startPos);
    const QTextLine endLine = endLayout->lineForTextPosition(endPos);

    const double x = startBoundingRect.x() + startLine.cursorToX(startPos);
    const double y = startBoundingRect.y() + startLine.y();
    const double r = endBoundingRect.x() + endLine.cursorToX(endPos);
    const double b = endBoundingRect.y() + endLine.y() + endLine.height();

    const int offset = qRound(y) % qRound(pageSize.height());

    if(x > r)
    {  // line break, so return a pseudo character on the start line
        rect = QRectF(x / pageSize.width(), offset / pageSize.height(),
                      3 / pageSize.width(),
                      startLine.height() / pageSize.height());
        page = -1;
        return;
    }

    page = qRound(y) / qRound(pageSize.height());
    rect = QRectF(x / pageSize.width(), offset / pageSize.height(),
                  (r - x) / pageSize.width(), (b - y) / pageSize.height());
}

static QVector<QRectF> calculateBoundingRects(QTextDocument* document,
                                              int startPosition,
                                              int endPosition)
{
    QVector<QRectF> result;

    const QSizeF pageSize = document->pageSize();

    const QTextBlock startBlock = document->findBlock(startPosition);
    const QRectF startBoundingRect =
        document->documentLayout()->blockBoundingRect(startBlock);

    const QTextBlock endBlock = document->findBlock(endPosition);
    const QRectF endBoundingRect =
        document->documentLayout()->blockBoundingRect(endBlock);

    const QTextLayout* startLayout = startBlock.layout();
    const QTextLayout* endLayout = endBlock.layout();
    if(!startLayout || !endLayout)
    {
        qCWarning(OkularCoreDebug)
            << "Start or end layout not found" << startLayout << endLayout;
        return {};
    }

    const int startPos = startPosition - startBlock.position();
    const int endPos = endPosition - endBlock.position();
    const QTextLine startLine = startLayout->lineForTextPosition(startPos);
    const QTextLine endLine = endLayout->lineForTextPosition(endPos);

    // This only works if both start and end layout are the same
    if(startLayout == endLayout)
    {
        Q_ASSERT(startBoundingRect == endBoundingRect);
        for(int i = startLine.lineNumber(); i < endLine.lineNumber(); ++i)
        {
            const QTextLine line = startLayout->lineAt(i);
            // using startPos and endPos is fine, if the pos is out of bounds
            // for that line, it'll return beginning and end of line
            // respectively
            const double x = endBoundingRect.x() + line.cursorToX(startPos);
            const double y = endBoundingRect.y() + line.y();
            const double r = endBoundingRect.x() + line.cursorToX(endPos);
            const double b = endBoundingRect.y() + line.y() + endLine.height();

            result.append(QRectF(x / pageSize.width(), y / pageSize.height(),
                                 (r - x) / pageSize.width(),
                                 (b - y) / pageSize.height()));
        }

        // The last line
        const double x = endBoundingRect.x() + endLine.cursorToX(startPos);
        const double y = endBoundingRect.y() + endLine.y();
        const double r = endBoundingRect.x() + endLine.cursorToX(endPos);
        const double b = endBoundingRect.y() + endLine.y() + endLine.height();

        result.append(QRectF(x / pageSize.width(), y / pageSize.height(),
                             (r - x) / pageSize.width(),
                             (b - y) / pageSize.height()));
    }
    else
    {
        const double x = startBoundingRect.x() + startLine.cursorToX(startPos);
        const double y = startBoundingRect.y() + startLine.y();
        const double r = endBoundingRect.x() + endLine.cursorToX(endPos);
        const double b = endBoundingRect.y() + endLine.y() + endLine.height();

        result.append(QRectF(x / pageSize.width(), y / pageSize.height(),
                             (r - x) / pageSize.width(),
                             (b - y) / pageSize.height()));
    }

    return result;
}

static void calculatePositions(QTextDocument* document, int page, int& start,
                               int& end)
{
    const QAbstractTextDocumentLayout* layout = document->documentLayout();
    const QSizeF pageSize = document->pageSize();
    const double margin = document->rootFrame()->frameFormat().margin();

    /**
     * Take the upper left and lower left corner including the margin
     */
    start = layout->hitTest(
        QPointF(margin, (page * pageSize.height()) + margin), Qt::FuzzyHit);
    end = layout->hitTest(
        QPointF(margin, ((page + 1) * pageSize.height()) - margin),
        Qt::FuzzyHit);
}

static Okular::DocumentViewport calculateViewport(QTextDocument* document,
                                                  const QTextBlock& block)
{
    const QSizeF pageSize = document->pageSize();
    const QRectF rect = document->documentLayout()->blockBoundingRect(block);

    const int page = qRound(rect.y()) / qRound(pageSize.height());
    const int offset = qRound(rect.y()) % qRound(pageSize.height());

    Okular::DocumentViewport viewport(page);
    viewport.rePos.normalizedX = (double)rect.x() / (double)pageSize.width();
    viewport.rePos.normalizedY = (double)offset / (double)pageSize.height();
    viewport.rePos.enabled = true;
    viewport.rePos.pos = Okular::DocumentViewport::Center;

    return viewport;
}
}  // namespace TextDocumentUtils

class TextDocumentConverterPrivate
{
public:
    TextDocumentConverterPrivate() :
        mParent(nullptr)
    {
    }

    TextDocumentGeneratorPrivate* mParent;
    QTextDocument* mDocument;
};

class TextDocumentGeneratorPrivate : public GeneratorPrivate
{
    friend class TextDocumentConverter;

public:
    explicit TextDocumentGeneratorPrivate(TextDocumentConverter* converter) :
        mConverter(converter),
        mDocument(nullptr),
        mGeneralSettings(nullptr)
    {
    }

    ~TextDocumentGeneratorPrivate() override
    {
        delete mConverter;
        delete mDocument;
    }

    void initializeGenerator();

    struct LinkInfo
    {
        int page;
        QRectF boundingRect;
        Action* link;
        bool ownsLink;
    };

    struct AnnotationInfo
    {
        int page;
        QRectF boundingRect;
        Annotation* annotation;
    };

    Q_DECLARE_PUBLIC(TextDocumentGenerator)

    /* reimp */ QVariant metaData(const QString& key,
                                  const QVariant& option) const override;
    /* reimp */ QImage image(PixmapRequest*) override;

    void calculateBoundingRect(int startPosition, int endPosition, QRectF& rect,
                               int& page) const;
    void calculatePositions(int page, int& start, int& end) const;
    Okular::TextPage* createTextPage(int) const;

    void addAction(Action* action, int cursorBegin, int cursorEnd);
    void addAnnotation(Annotation* annotation, int cursorBegin, int cursorEnd);
    void addTitle(int level, const QString& title, const QTextBlock& block);
    void addMetaData(const QString& key, const QString& value,
                     const QString& title);
    void addMetaData(DocumentInfo::Key, const QString& value);

    QList<LinkInfo> generateLinkInfos() const;
    QList<AnnotationInfo> generateAnnotationInfos() const;
    void generateTitleInfos();

    TextDocumentConverter* mConverter;

    QTextDocument* mDocument;
    Okular::DocumentInfo mDocumentInfo;
    Okular::DocumentSynopsis mDocumentSynopsis;

    struct TitlePosition
    {
        int level;
        QString title;
        QTextBlock block;
    };

    QList<TitlePosition> mTitlePositions;

    struct LinkPosition
    {
        int startPosition;
        int endPosition;
        Action* link;
    };

    QList<LinkPosition> mLinkPositions;

    struct AnnotationPosition
    {
        int startPosition;
        int endPosition;
        Annotation* annotation;
    };

    QList<AnnotationPosition> mAnnotationPositions;

    TextDocumentSettings* mGeneralSettings;

    QFont mFont;
};

}  // namespace Okular

#endif
