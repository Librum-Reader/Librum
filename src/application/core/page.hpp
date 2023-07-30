#pragma once
#include <QImage>
#include <QList>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QString>
#include "application_export.hpp"
#include "document.hpp"
#include "mupdf/classes.h"

namespace application::core
{

class APPLICATION_LIBRARY Page
{
public:
    Page(const Document* document, int pageNumber);
    void setupDisplayList(const mupdf::FzRect& boundPage);
    void setupTextPage(int pageNumber);
    void setupSymbolBounds();

    int getWidth() const;
    int getHeight() const;

    float getZoom() const;
    void setZoom(float newZoom);

    QList<QRectF>& getBufferedSelectionRects();

    QImage renderPage();
    QPointF scalePointToCurrentZoom(const QPointF& point, float oldZoom);
    void setInvertColor(bool newInvertColor);
    void generateSelectionRects(QPointF start, QPointF end);
    QRectF getRectForWord(QPointF wordPos);
    QString getTextFromSelection(const QPointF& start, const QPointF& end);
    bool pointIsAboveText(const QPoint& point);

private:
    mupdf::FzPixmap getEmptyPixmap() const;
    QImage imageFromPixmap(mupdf::FzPixmap pixmap);
    QRectF fzQuadToQRectF(const mupdf::FzQuad& rect);

    const mupdf::FzDocument* m_document;
    std::unique_ptr<mupdf::FzPage> m_page;
    std::unique_ptr<mupdf::FzStextPage> m_textPage;
    mupdf::FzDisplayList m_displayList;
    mupdf::FzMatrix m_matrix;
    QList<QRectF> m_bufferedSelectionRects;
    std::vector<fz_rect> m_pageSymbolBounds;
    bool m_invertColor = false;

    bool m_pageImageOutdated = true;
    QImage m_pageImage;
};

}  // namespace application::core
