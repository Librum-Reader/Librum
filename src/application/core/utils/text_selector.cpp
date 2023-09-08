#include "text_selector.hpp"
#include <mupdf/functions.h>

namespace application::core::utils
{

TextSelector::TextSelector(mupdf::FzStextPage* textPage,
                           mupdf::FzMatrix* matrix) :
    m_textPage(textPage),
    m_matrix(matrix)
{
}

void TextSelector::generateSelectionRects(QList<mupdf::FzQuad>& container,
                                          mupdf::FzPoint start,
                                          mupdf::FzPoint end)
{
    restorePoint(start);
    restorePoint(end);

    const int max = 1000;
    fz_quad hits[max];
    int n = mupdf::ll_fz_highlight_selection(
        m_textPage->m_internal, *start.internal(), *end.internal(), hits, max);

    for(int i = 0; i < n; ++i)
    {
        fz_quad hit = hits[i];
        hit = mupdf::ll_fz_transform_quad(hit, *m_matrix->internal());
        container.append(hit);
    }
}

QPair<mupdf::FzPoint, mupdf::FzPoint>
    TextSelector::getPositionsForWordSelection(mupdf::FzPoint start,
                                               mupdf::FzPoint end)
{
    restorePoint(start);
    restorePoint(end);

    // This modifies the fzBegin and fzEnd.
    mupdf::ll_fz_snap_selection(m_textPage->m_internal, start.internal(),
                                end.internal(), FZ_SELECT_WORDS);

    start = start.fz_transform_point(*m_matrix);
    end = end.fz_transform_point(*m_matrix);

    return QPair<mupdf::FzPoint, mupdf::FzPoint>(
        mupdf::FzPoint(start.x, start.y), mupdf::FzPoint(end.x, end.y));
}

QPair<mupdf::FzPoint, mupdf::FzPoint>
    TextSelector::getPositionsForLineSelection(mupdf::FzPoint point)
{
    restorePoint(point);
    mupdf::FzPoint begin(point.x, point.y);
    mupdf::FzPoint end = begin;

    // This modifies the fzPoint
    mupdf::ll_fz_snap_selection(m_textPage->m_internal, begin.internal(),
                                end.internal(), FZ_SELECT_LINES);

    begin = begin.fz_transform_point(*m_matrix);
    end = end.fz_transform_point(*m_matrix);

    return QPair<mupdf::FzPoint, mupdf::FzPoint>(
        mupdf::FzPoint(begin.x, begin.y), mupdf::FzPoint(end.x, end.y));
}

std::string TextSelector::getTextFromSelection(mupdf::FzPoint start,
                                               mupdf::FzPoint end)
{
    restorePoint(start);
    restorePoint(end);

    auto text = m_textPage->fz_copy_selection(start, end, 1);
    return text;
}

/**
 * Restores the point to its original position by applying the inverse
 * transformation matrix to it. This is necessary because mupdf expects
 * arguments passed to be without any zooming or similar applied.
 */
void TextSelector::restorePoint(mupdf::FzPoint& point)
{
    auto invMatrix = m_matrix->fz_invert_matrix();
    point = point.fz_transform_point(invMatrix);
}

}  // namespace application::core::utils