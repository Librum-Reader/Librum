#pragma once
#include <QImage>
#include <QList>
#include <QPointF>
#include <QRectF>
#include "application_export.hpp"
#include "document.hpp"
#include "mupdf/classes.h"

namespace application::core
{

class APPLICATION_LIBRARY Page
{
public:
    Page(const Document* document, int pageNumber);

    QImage renderPage();
    int getWidth() const;
    int getHeight() const;
    QList<QRectF>& getBufferedHighlights();

    QPointF scalePointToZoom(const QPointF& point, float zoom);

    void setZoom(float newZoom);
    void setInvertColor(bool newInvertColor);
    void setHighlight(QPointF start, QPointF end);

private:
    mupdf::FzPixmap getEmptyPixmap() const;
    QImage imageFromPixmap(mupdf::FzPixmap pixmap);

    const mupdf::FzDocument* m_document;
    std::unique_ptr<mupdf::FzPage> m_page;
    std::unique_ptr<mupdf::FzStextPage> m_textPage;
    mupdf::FzDisplayList m_displayList;
    mupdf::FzMatrix m_matrix;
    QList<QRectF> m_bufferedHighlights;
    bool m_invertColor = false;

    bool m_pageImageInvalid = true;
    QImage m_pageImage;
};

}  // namespace application::core
