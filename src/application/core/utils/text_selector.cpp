#include "text_selector.hpp"
#include <mupdf/functions.h>

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
    const int max = 1000;
    fz_quad hits[max];
    int n = mupdf::ll_fz_highlight_selection(
        m_textPage->m_internal, *start.internal(), *end.internal(), hits, max);

    for(int i = 0; i < n; ++i)
    {
        fz_quad hit = hits[i];
        container.append(hit);
    }
}

FzPointPair TextSelector::getPositionsForWordSelection(mupdf::FzPoint start,
                                                       mupdf::FzPoint end)
{
    // This modifies the fzBegin and fzEnd.
    mupdf::ll_fz_snap_selection(m_textPage->m_internal, start.internal(),
                                end.internal(), FZ_SELECT_WORDS);

    return { start, end };
}

FzPointPair TextSelector::getPositionsForLineSelection(mupdf::FzPoint point)
{
    mupdf::FzPoint start(point.x, point.y);
    mupdf::FzPoint end = start;

    // This modifies the fzPoint
    mupdf::ll_fz_snap_selection(m_textPage->m_internal, start.internal(),
                                end.internal(), FZ_SELECT_LINES);

    return { start, end };
}

std::string TextSelector::getTextFromSelection(mupdf::FzPoint start,
                                               mupdf::FzPoint end)
{
    auto text = m_textPage->fz_copy_selection(start, end, 1);
    return text;
}

}  // namespace application::core::utils