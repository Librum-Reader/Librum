#pragma once
#include <QImage>
#include <QList>
#include <QPair>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QString>
#include "application_export.hpp"
#include "mupdf/classes.h"

namespace application::core
{

class APPLICATION_EXPORT PageGenerator
{
public:
    PageGenerator(mupdf::FzDocument* document, int pageNumber);

    int getWidth() const;
    int getHeight() const;

    float getZoom() const;
    void setZoom(float newZoom);

    void setInvertColor(bool newInvertColor);

    QList<QRectF>& getBufferedSelectionRects();
    void generateSelectionRects(QPointF start, QPointF end);

    QImage renderPage();

    bool pointIsAboveText(const QPoint& point);
    bool pointIsAboveLink(const QPoint& point);
    mupdf::FzLink getLinkAtPoint(const QPoint& point);

    QPair<QPointF, QPointF> getPositionsForWordSelection(QPointF begin,
                                                         QPointF end);
    QPair<QPointF, QPointF> getPositionsForLineSelection(QPointF point);
    QString getTextFromSelection(const QPointF& start, const QPointF& end);

private:
    void setupDisplayList(const mupdf::FzRect& boundPage);
    void setupTextPage(int pageNumber);
    void setupSymbolBounds();
    void setupLinks();
    mupdf::FzPixmap getEmptyPixmap() const;

    const mupdf::FzDocument* m_document;
    std::unique_ptr<mupdf::FzPage> m_page;
    std::unique_ptr<mupdf::FzStextPage> m_textPage;
    mupdf::FzDisplayList m_displayList;
    mupdf::FzMatrix m_matrix;
    QList<QRectF> m_bufferedSelectionRects;
    QList<mupdf::FzLink> m_links;
    std::vector<fz_rect> m_symbolBounds;
    bool m_invertColor = false;

    bool m_pageImageOutdated = true;
    QImage m_pageImage;
};

}  // namespace application::core
