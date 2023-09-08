#pragma once
#include <QList>
#include "mupdf/classes.h"

namespace application::core::utils
{

class TextSelector
{
public:
    TextSelector(mupdf::FzStextPage* textPage, mupdf::FzMatrix* matrix);

    void generateSelectionRects(QList<mupdf::FzQuad>& container,
                                mupdf::FzPoint start, mupdf::FzPoint end);
    QPair<mupdf::FzPoint, mupdf::FzPoint> getPositionsForWordSelection(
        mupdf::FzPoint start, mupdf::FzPoint end);
    QPair<mupdf::FzPoint, mupdf::FzPoint> getPositionsForLineSelection(
        mupdf::FzPoint point);
    std::string getTextFromSelection(const mupdf::FzPoint& start,
                                     const mupdf::FzPoint& end);

private:
    void normalizePoint(mupdf::FzPoint& point);

    mupdf::FzStextPage* m_textPage;
    mupdf::FzMatrix* m_matrix;
};

}  // namespace application::core::utils