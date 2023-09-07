#include "page_generator.hpp"
#include <cmath>
#include "mupdf/classes.h"
#include "mupdf/classes2.h"
#include "mupdf/functions.h"
#include "utils/fz_utils.hpp"

namespace application::core
{

application::core::PageGenerator::PageGenerator(mupdf::FzDocument* document,
                                                int pageNumber) :
    m_document(document)
{
    m_page =
        std::make_unique<mupdf::FzPage>(m_document->fz_load_page(pageNumber));
    auto boundPage = m_page->fz_bound_page();

    setupDisplayList(boundPage);
    setupTextPage(pageNumber);
    setupSymbolBounds();
    setupLinks();
}

void PageGenerator::setupDisplayList(const mupdf::FzRect& boundPage)
{
    m_displayList = mupdf::FzDisplayList(boundPage);

    auto listDevice = m_displayList.fz_new_list_device();
    mupdf::FzCookie defaultCookie;
    m_page->fz_run_page(listDevice, mupdf::FzMatrix(), defaultCookie);
    listDevice.fz_close_device();
}

void PageGenerator::setupTextPage(int pageNumber)
{
    mupdf::FzStextOptions options;
    m_textPage =
        std::make_unique<mupdf::FzStextPage>(*m_document, pageNumber, options);
}

void PageGenerator::setupSymbolBounds()
{
    auto curr = m_textPage->begin();
    auto end = m_textPage->end();

    m_symbolBounds.resize(100);
    while(curr != end)
    {
        auto symbol = curr;
        fz_rect rect = symbol->m_internal->bbox;
        m_symbolBounds.emplace_back(rect);
        ++curr;
    }
}

void PageGenerator::setupLinks()
{
    auto end = m_page->fz_load_links().end();
    auto curr = m_page->fz_load_links().begin();
    while(curr != end)
    {
        m_links.push_back(*curr);
        ++curr;
    }
}

mupdf::FzPixmap PageGenerator::renderPage()
{
    if(!m_pixmapOutdated)
        return m_pixmap;

    m_pixmap = getEmptyPixmap();
    auto drawDevice = mupdf::fz_new_draw_device(mupdf::FzMatrix(), m_pixmap);

    mupdf::FzCookie cookie;
    mupdf::FzRect rect = mupdf::FzRect::Fixed_INFINITE;
    m_displayList.fz_run_display_list(drawDevice, m_matrix, rect, cookie);
    drawDevice.fz_close_device();

    if(m_invertColor)
        m_pixmap.fz_invert_pixmap();

    m_pixmapOutdated = false;
    return m_pixmap;
}

mupdf::FzPixmap PageGenerator::getEmptyPixmap() const
{
    auto bbox = m_page->fz_bound_page_box(FZ_CROP_BOX);
    auto scaledBbox = bbox.fz_transform_rect(m_matrix);

    mupdf::FzPixmap pixmap(mupdf::FzColorspace::Fixed_RGB, scaledBbox,
                           mupdf::FzSeparations(), 0);
    pixmap.fz_clear_pixmap();

    return pixmap;
}

void imageCleanupHandler(void* data)
{
    unsigned char* samples = static_cast<unsigned char*>(data);
    delete[] samples;
}

int PageGenerator::getWidth() const
{
    auto bbox = m_page->fz_bound_page_box(FZ_CROP_BOX);

    return (bbox.x1 - bbox.x0) * m_matrix.a;
}

int PageGenerator::getHeight() const
{
    auto bbox = m_page->fz_bound_page_box(FZ_CROP_BOX);

    return (bbox.y1 - bbox.y0) * m_matrix.d;
}

float PageGenerator::getZoom() const
{
    return m_matrix.a;
}

QList<mupdf::FzQuad>& PageGenerator::getBufferedSelectionRects()
{
    return m_bufferedSelectionRects;
}

void PageGenerator::setZoom(float newZoom)
{
    if(m_matrix.a == newZoom && m_matrix.d == newZoom)
        return;

    m_matrix.a = newZoom;
    m_matrix.d = newZoom;
    m_pixmapOutdated = true;
}

void PageGenerator::setInvertColor(bool newInvertColor)
{
    if(m_invertColor == newInvertColor)
        return;

    m_invertColor = newInvertColor;
    m_pixmapOutdated = true;
}

void PageGenerator::generateSelectionRects(mupdf::FzPoint start,
                                           mupdf::FzPoint end)
{
    mupdf::FzPoint fzStart(start.x, start.y);
    mupdf::FzPoint fzEnd(end.x, end.y);

    auto invMatrix = m_matrix.fz_invert_matrix();
    auto normStart = fzStart.fz_transform_point(invMatrix);
    auto normEnd = fzEnd.fz_transform_point(invMatrix);

    const int max = 1000;
    fz_quad hits[max];
    int n = mupdf::ll_fz_highlight_selection(m_textPage->m_internal,
                                             *normStart.internal(),
                                             *normEnd.internal(), hits, max);

    for(int i = 0; i < n; ++i)
    {
        fz_quad hit = hits[i];
        hit = mupdf::ll_fz_transform_quad(hit, *m_matrix.internal());
        m_bufferedSelectionRects.append(hit);
    }
}

QPair<mupdf::FzPoint, mupdf::FzPoint>
    PageGenerator::getPositionsForWordSelection(mupdf::FzPoint begin,
                                                mupdf::FzPoint end)
{
    mupdf::FzPoint fzBegin(begin.x, begin.y);
    mupdf::FzPoint fzEnd(end.x, end.y);
    auto invMatrix = m_matrix.fz_invert_matrix();
    fzBegin = fzBegin.fz_transform_point(invMatrix);
    fzEnd = fzEnd.fz_transform_point(invMatrix);

    // This modifies the fzBegin and fzEnd.
    mupdf::ll_fz_snap_selection(m_textPage->m_internal, fzBegin.internal(),
                                fzEnd.internal(), FZ_SELECT_WORDS);

    fzBegin = fzBegin.fz_transform_point(m_matrix);
    fzEnd = fzEnd.fz_transform_point(m_matrix);

    return QPair<mupdf::FzPoint, mupdf::FzPoint>(
        mupdf::FzPoint(fzBegin.x, fzBegin.y), mupdf::FzPoint(fzEnd.x, fzEnd.y));
}

QPair<mupdf::FzPoint, mupdf::FzPoint>
    PageGenerator::getPositionsForLineSelection(mupdf::FzPoint point)
{
    mupdf::FzPoint fzBegin(point.x, point.y);
    mupdf::FzPoint fzEnd(point.x, point.y);
    auto invMatrix = m_matrix.fz_invert_matrix();
    fzBegin = fzBegin.fz_transform_point(invMatrix);
    fzEnd = fzEnd.fz_transform_point(invMatrix);

    // This modifies the fzPoint
    mupdf::ll_fz_snap_selection(m_textPage->m_internal, fzBegin.internal(),
                                fzEnd.internal(), FZ_SELECT_LINES);

    fzBegin = fzBegin.fz_transform_point(m_matrix);
    fzEnd = fzEnd.fz_transform_point(m_matrix);

    return QPair<mupdf::FzPoint, mupdf::FzPoint>(
        mupdf::FzPoint(fzBegin.x, fzBegin.y), mupdf::FzPoint(fzEnd.x, fzEnd.y));
}

bool PageGenerator::pointIsAboveText(const mupdf::FzPoint& point)
{
    mupdf::FzPoint fzPoint(point.x, point.y);
    fzPoint = fzPoint.transform(m_matrix.fz_invert_matrix());

    for(auto& rect : m_symbolBounds)
    {
        if(fzPoint.fz_is_point_inside_rect(rect))
            return true;
    }

    return false;
}

bool PageGenerator::pointIsAboveLink(const mupdf::FzPoint& point)
{
    mupdf::FzPoint fzPoint(point.x, point.y);
    fzPoint = fzPoint.transform(m_matrix.fz_invert_matrix());

    for(auto& link : m_links)
    {
        if(fzPoint.fz_is_point_inside_rect(link.rect()))
            return true;
    }

    return false;
}

mupdf::FzLink PageGenerator::getLinkAtPoint(const mupdf::FzPoint& point)
{
    mupdf::FzPoint fzPoint(point.x, point.y);
    fzPoint = fzPoint.transform(m_matrix.fz_invert_matrix());

    for(auto& link : m_links)
    {
        if(fzPoint.fz_is_point_inside_rect(link.rect()))
            return link;
    }

    return mupdf::FzLink();
}

std::string PageGenerator::getTextFromSelection(const mupdf::FzPoint& start,
                                                const mupdf::FzPoint& end)
{
    mupdf::FzPoint fzStart(start.x, start.y);
    mupdf::FzPoint fzEnd(end.x, end.y);
    fzStart = fzStart.transform(m_matrix.fz_invert_matrix());
    fzEnd = fzEnd.transform(m_matrix.fz_invert_matrix());

    auto text = m_textPage->fz_copy_selection(fzStart, fzEnd, 1);
    return text;
}

}  // namespace application::core