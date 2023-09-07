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
    mupdf::FzPoint fzStart(start.x, start.y);
    mupdf::FzPoint fzEnd(end.x, end.y);

    auto invMatrix = m_matrix->fz_invert_matrix();
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
        hit = mupdf::ll_fz_transform_quad(hit, *m_matrix->internal());
        container.append(hit);
    }
}

QPair<mupdf::FzPoint, mupdf::FzPoint>
    TextSelector::getPositionsForWordSelection(mupdf::FzPoint begin,
                                               mupdf::FzPoint end)
{
    mupdf::FzPoint fzBegin(begin.x, begin.y);
    mupdf::FzPoint fzEnd(end.x, end.y);
    auto invMatrix = m_matrix->fz_invert_matrix();
    fzBegin = fzBegin.fz_transform_point(invMatrix);
    fzEnd = fzEnd.fz_transform_point(invMatrix);

    // This modifies the fzBegin and fzEnd.
    mupdf::ll_fz_snap_selection(m_textPage->m_internal, fzBegin.internal(),
                                fzEnd.internal(), FZ_SELECT_WORDS);

    fzBegin = fzBegin.fz_transform_point(*m_matrix);
    fzEnd = fzEnd.fz_transform_point(*m_matrix);

    return QPair<mupdf::FzPoint, mupdf::FzPoint>(
        mupdf::FzPoint(fzBegin.x, fzBegin.y), mupdf::FzPoint(fzEnd.x, fzEnd.y));
}

QPair<mupdf::FzPoint, mupdf::FzPoint>
    TextSelector::getPositionsForLineSelection(mupdf::FzPoint point)
{
    mupdf::FzPoint fzBegin(point.x, point.y);
    mupdf::FzPoint fzEnd(point.x, point.y);
    auto invMatrix = m_matrix->fz_invert_matrix();
    fzBegin = fzBegin.fz_transform_point(invMatrix);
    fzEnd = fzEnd.fz_transform_point(invMatrix);

    // This modifies the fzPoint
    mupdf::ll_fz_snap_selection(m_textPage->m_internal, fzBegin.internal(),
                                fzEnd.internal(), FZ_SELECT_LINES);

    fzBegin = fzBegin.fz_transform_point(*m_matrix);
    fzEnd = fzEnd.fz_transform_point(*m_matrix);

    return QPair<mupdf::FzPoint, mupdf::FzPoint>(
        mupdf::FzPoint(fzBegin.x, fzBegin.y), mupdf::FzPoint(fzEnd.x, fzEnd.y));
}

std::string TextSelector::getTextFromSelection(const mupdf::FzPoint& start,
                                               const mupdf::FzPoint& end)
{
    mupdf::FzPoint fzStart(start.x, start.y);
    mupdf::FzPoint fzEnd(end.x, end.y);
    fzStart = fzStart.transform(m_matrix->fz_invert_matrix());
    fzEnd = fzEnd.transform(m_matrix->fz_invert_matrix());

    auto text = m_textPage->fz_copy_selection(fzStart, fzEnd, 1);
    return text;
}

}  // namespace application::core::utils