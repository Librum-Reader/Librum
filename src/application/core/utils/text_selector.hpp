#pragma once
#include <QList>
#include <QPair>
#include "fz_utils.hpp"
#include "mupdf/classes.h"

namespace application::core::utils
{

class TextSelector
{

public:
    TextSelector(mupdf::FzStextPage* textPage);

    void generateSelectionRects(QList<mupdf::FzQuad>& container,
                                mupdf::FzPoint start, mupdf::FzPoint end);
    FzPointPair getPositionsForWordSelection(mupdf::FzPoint start,
                                             mupdf::FzPoint end);
    FzPointPair getPositionsForLineSelection(mupdf::FzPoint point);
    std::string getTextFromSelection(mupdf::FzPoint start, mupdf::FzPoint end);

    void setPageXOffset(int newXOffset);
    void setPageYOffset(int newYOffset);

private:
    mupdf::FzStextPage* m_textPage;
    int m_pageXOffset = 0;
    int m_pageYOffset = 0;
};

}  // namespace application::core::utils