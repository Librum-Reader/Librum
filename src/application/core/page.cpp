#include "page.hpp"
#include <QDebug>
#include <cmath>
#include "mupdf/classes.h"
#include "mupdf/classes2.h"
#include "mupdf/functions.h"

namespace application::core
{

application::core::Page::Page(const Document* document, int pageNumber) :
    m_document(document->internal())
{
    m_page =
        std::make_unique<mupdf::FzPage>(m_document->fz_load_page(pageNumber));
    auto boundPage = m_page->fz_bound_page();

    m_displayList = mupdf::FzDisplayList(boundPage);
    auto listDevice = m_displayList.fz_new_list_device();
    mupdf::FzCookie defaultCookie;
    m_page->fz_run_page(listDevice, mupdf::FzMatrix(), defaultCookie);
    listDevice.fz_close_device();

    mupdf::FzStextOptions options(
        FZ_STEXT_PRESERVE_LIGATURES | FZ_STEXT_PRESERVE_WHITESPACE |
        FZ_STEXT_PRESERVE_IMAGES | FZ_STEXT_PRESERVE_SPANS);
    m_textPage =
        std::make_unique<mupdf::FzStextPage>(*m_document, pageNumber, options);

    fz_quad hits[1000];
    fz_point topLeft(boundPage.x0, boundPage.y0);
    fz_point bottomRight(boundPage.x1, boundPage.y1);
    int n = mupdf::ll_fz_highlight_selection(m_textPage->m_internal, topLeft,
                                             bottomRight, hits, 1000);
    m_allTextQuads = std::vector<fz_quad>(hits, hits + n);
}

QImage Page::renderPage()
{
    if(!m_pageImageInvalid)
        return m_pageImage;

    auto pixmap = getEmptyPixmap();
    auto drawDevice = mupdf::fz_new_draw_device(mupdf::FzMatrix(), pixmap);

    mupdf::FzCookie cookie;
    mupdf::FzRect rect = mupdf::FzRect::Fixed_INFINITE;
    m_displayList.fz_run_display_list(drawDevice, m_matrix, rect, cookie);
    drawDevice.fz_close_device();

    if(m_invertColor)
        pixmap.fz_invert_pixmap();

    m_pageImage = imageFromPixmap(pixmap);
    m_pageImageInvalid = false;
    return m_pageImage;
}

mupdf::FzPixmap Page::getEmptyPixmap() const
{
    auto bbox = m_page->fz_bound_page_box(FZ_CROP_BOX);
    bbox = bbox.fz_transform_rect(m_matrix);

    mupdf::FzPixmap pixmap(mupdf::FzColorspace::Fixed_RGB, bbox,
                           mupdf::FzSeparations(), 0);
    pixmap.fz_clear_pixmap();

    return pixmap;
}

void imageCleanupHandler(void* data)
{
    unsigned char* samples = static_cast<unsigned char*>(data);
    delete[] samples;
}

QImage Page::imageFromPixmap(mupdf::FzPixmap pixmap)
{
    int width = pixmap.w();
    int height = pixmap.h();
    int stride = pixmap.stride();
    auto samples = pixmap.samples();

    // Copy samples
    std::size_t length = stride * height;
    unsigned char* destination = new unsigned char[length + 1];
    std::copy(samples, samples + length + 1, destination);

    QImage image(destination, width, height, stride, QImage::Format_RGB888,
                 imageCleanupHandler, destination);
    return image;
}

int Page::getWidth() const
{
    auto bbox = m_page->fz_bound_page_box(FZ_CROP_BOX);

    return (bbox.x1 - bbox.x0) * m_matrix.a;
}

int Page::getHeight() const
{
    auto bbox = m_page->fz_bound_page_box(FZ_CROP_BOX);

    return (bbox.y1 - bbox.y0) * m_matrix.d;
}

QList<QRectF>& Page::getBufferedHighlights()
{
    return m_bufferedHighlights;
}

QPointF Page::scalePointToZoom(const QPointF& point, float zoom)
{
    mupdf::FzPoint fzPoint(point.x(), point.y());

    // Normalize
    auto invMatrix = m_matrix.fz_invert_matrix();
    fzPoint = fzPoint.fz_transform_point(invMatrix);

    // Apply new zoom
    mupdf::FzMatrix newMatrix;
    newMatrix.a = zoom;
    newMatrix.d = zoom;
    fzPoint = fzPoint.fz_transform_point(newMatrix);

    return QPointF(fzPoint.x, fzPoint.y);
}

void Page::setZoom(float newZoom)
{
    if(m_matrix.a == newZoom && m_matrix.d == newZoom)
        return;

    m_matrix.a = newZoom;
    m_matrix.d = newZoom;
    m_pageImageInvalid = true;
}

void Page::setInvertColor(bool newInvertColor)
{
    if(m_invertColor == newInvertColor)
        return;

    m_invertColor = newInvertColor;
    m_pageImageInvalid = true;
}

void Page::setHighlight(QPointF start, QPointF end)
{
    mupdf::FzPoint fzStart(start.x(), start.y());
    mupdf::FzPoint fzEnd(end.x(), end.y());
    auto invMatrix = m_matrix.fz_invert_matrix();
    fzStart = fzStart.fz_transform_point(invMatrix);
    fzEnd = fzEnd.fz_transform_point(invMatrix);

    fz_quad hits[1000];
    int n = mupdf::ll_fz_highlight_selection(m_textPage->m_internal,
                                             *fzStart.internal(),
                                             *fzEnd.internal(), hits, 1000);

    for(int i = 0; i < n; ++i)
    {
        fz_quad hit = hits[i];
        hit = mupdf::ll_fz_transform_quad(hit, *m_matrix.internal());
        QRectF rect(hit.ul.x, hit.ul.y, hit.ur.x - hit.ul.x,
                    hit.ll.y - hit.ul.y);
        m_bufferedHighlights.append(rect);
    }
}

bool Page::textIsBelowPoint(const QPoint& point)
{
    mupdf::FzPoint fzPoint(point.x(), point.y());
    fzPoint = fzPoint.transform(m_matrix.fz_invert_matrix());

    for(auto& quad : m_allTextQuads)
    {
        if(fzPoint.fz_is_point_inside_quad(quad))
            return true;
    }

    return false;
}

QString Page::getTextFromCurrentHighlight(const QPointF& start,
                                          const QPointF& end)
{
    mupdf::FzPoint fzStart(start.x(), start.y());
    mupdf::FzPoint fzEnd(end.x(), end.y());
    fzStart = fzStart.transform(m_matrix.fz_invert_matrix());
    fzEnd = fzEnd.transform(m_matrix.fz_invert_matrix());

    auto result = m_textPage->fz_copy_selection(fzStart, fzEnd, 1);

    return QString::fromStdString(result);
}

}  // namespace application::core