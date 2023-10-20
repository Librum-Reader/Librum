#include "text_selector.hpp"
#include <mupdf/functions.h>
#include <QDebug>
#include "fz_utils.hpp"
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
    start = utils::movePoint(start, m_pageXOffset, m_pageYOffset);
    end = utils::movePoint(end, m_pageXOffset, m_pageYOffset);

    const int max = 1000;
    fz_quad hits[max];
    int n = mupdf::ll_fz_highlight_selection(
        m_textPage->m_internal, *start.internal(), *end.internal(), hits, max);

    for(int i = 0; i < n; ++i)
    {
        fz_quad hit = hits[i];
        hit = utils::moveQuad(hit, m_pageXOffset, m_pageYOffset);

        container.append(hit);
    }
}

FzPointPair TextSelector::getPositionsForWordSelection(mupdf::FzPoint start,
                                                       mupdf::FzPoint end)
{
    start = utils::movePoint(start, m_pageXOffset, m_pageYOffset);
    end = utils::movePoint(end, m_pageXOffset, m_pageYOffset);

    // This modifies the fzBegin and fzEnd.
    mupdf::ll_fz_snap_selection(m_textPage->m_internal, start.internal(),
                                end.internal(), FZ_SELECT_WORDS);

    start = utils::movePoint(start, -m_pageXOffset, -m_pageYOffset);
    end = utils::movePoint(end, -m_pageXOffset, -m_pageYOffset);

    return { start, end };
}

FzPointPair TextSelector::getPositionsForLineSelection(mupdf::FzPoint point)
{
    auto start = utils::movePoint(point, m_pageXOffset, m_pageYOffset);
    auto end = utils::movePoint(point, m_pageXOffset, m_pageYOffset);

    // This modifies the fzPoint
    mupdf::ll_fz_snap_selection(m_textPage->m_internal, start.internal(),
                                end.internal(), FZ_SELECT_LINES);

    start = utils::movePoint(start, -m_pageXOffset, -m_pageYOffset);
    end = utils::movePoint(end, -m_pageXOffset, -m_pageYOffset);

    return { start, end };
}

std::string TextSelector::getTextFromSelection(mupdf::FzPoint start,
                                               mupdf::FzPoint end)
{
    start = utils::movePoint(start, m_pageXOffset, m_pageYOffset);
    end = utils::movePoint(end, m_pageXOffset, m_pageYOffset);

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