#include "text_selector.hpp"
#include <mupdf/functions.h>
#include <QDebug>
#include "mupdf/fitz/geometry.h"

namespace application::core::utils
{

TextSelector::TextSelector(mupdf::FzStextPage* textPage) :
    m_textPage(textPage)
{
}

void TextSelector::generateSelectionRects(QList<mupdf::FzQuad>& container,
                                          mupdf::FzPoint start,
                                          mupdf::FzPoint end)
{
    start = mupdf::ll_fz_make_point(start.x + m_pageXOffset,
                                    start.y + m_pageYOffset);
    end = mupdf::ll_fz_make_point(end.x + m_pageXOffset, end.y + m_pageYOffset);

    const int max = 1000;
    fz_quad hits[max];
    int n = mupdf::ll_fz_highlight_selection(
        m_textPage->m_internal, *start.internal(), *end.internal(), hits, max);

    for(int i = 0; i < n; ++i)
    {
        fz_quad hit = hits[i];
        // apply offsets to create new fz_quad
        hit = fz_make_quad(hit.ul.x - m_pageXOffset, hit.ul.y - m_pageYOffset,
                           hit.ur.x - m_pageXOffset, hit.ur.y - m_pageYOffset,
                           hit.ll.x - m_pageXOffset, hit.ll.y - m_pageYOffset,
                           hit.lr.x - m_pageXOffset, hit.lr.y - m_pageYOffset);
        container.append(hit);
    }
}

FzPointPair TextSelector::getPositionsForWordSelection(mupdf::FzPoint start,
                                                       mupdf::FzPoint end)
{
    start = mupdf::ll_fz_make_point(start.x + m_pageXOffset,
                                    start.y + m_pageYOffset);
    end = mupdf::ll_fz_make_point(end.x + m_pageXOffset, end.y + m_pageYOffset);

    // This modifies the fzBegin and fzEnd.
    mupdf::ll_fz_snap_selection(m_textPage->m_internal, start.internal(),
                                end.internal(), FZ_SELECT_WORDS);

    start = mupdf::ll_fz_make_point(start.x - m_pageXOffset,
                                    start.y - m_pageYOffset);
    end = mupdf::ll_fz_make_point(end.x - m_pageXOffset, end.y - m_pageYOffset);

    return { start, end };
}

FzPointPair TextSelector::getPositionsForLineSelection(mupdf::FzPoint point)
{
    mupdf::FzPoint start(point.x, point.y);
    mupdf::FzPoint end = start;

    start = mupdf::ll_fz_make_point(start.x + m_pageXOffset,
                                    start.y + m_pageYOffset);
    end = mupdf::ll_fz_make_point(end.x + m_pageXOffset, end.y + m_pageYOffset);

    // This modifies the fzPoint
    mupdf::ll_fz_snap_selection(m_textPage->m_internal, start.internal(),
                                end.internal(), FZ_SELECT_LINES);

    start = mupdf::ll_fz_make_point(start.x - m_pageXOffset,
                                    start.y - m_pageYOffset);
    end = mupdf::ll_fz_make_point(end.x - m_pageXOffset, end.y - m_pageYOffset);

    return { start, end };
}

std::string TextSelector::getTextFromSelection(mupdf::FzPoint start,
                                               mupdf::FzPoint end)
{
    start = mupdf::ll_fz_make_point(start.x + m_pageXOffset,
                                    start.y + m_pageYOffset);
    end = mupdf::ll_fz_make_point(end.x + m_pageXOffset, end.y + m_pageYOffset);

    auto text = m_textPage->fz_copy_selection(start, end, 1);
    return text;
}

void TextSelector::setPageXOffset(int newXOffset)
{
    m_pageXOffset = newXOffset;
}

void TextSelector::setPageYOffset(int newYOffset)
{
    m_pageYOffset = newYOffset;
}

}  // namespace application::core::utils