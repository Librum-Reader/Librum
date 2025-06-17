#pragma once
#include <QList>
#include <QPair>
#include <string>
#include <vector>
#include "application_export.hpp"
#include "fz_utils.hpp"
#include "mupdf/classes.h"
#include "text_selector.hpp"

namespace application::core
{

/**
 * The text generator is the low level class that is responsible to do any kind
 * of computation directly on the text, such as rendering the page, getting
 * positions of symbols, ...
 * It expects all coordinates to be "restored" meaning without any applied
 * transformations such as zooms.
 */
class APPLICATION_EXPORT PageGenerator
{
public:
    PageGenerator(mupdf::FzDocument* document, int pageNumber);

    int getWidth() const;
    int getHeight() const;

    int getPageXOffset() const;
    int getPageYOffset() const;

    mupdf::FzPixmap renderPage(float zoom);
    void setInvertColor(bool newInvertColor);

    bool pointIsAboveText(mupdf::FzPoint point);
    bool pointIsAboveLink(mupdf::FzPoint point);
    mupdf::FzLink getLinkAtPoint(mupdf::FzPoint point);

    QList<mupdf::FzQuad>& getBufferedSelectionRects();
    void generateSelectionRects(mupdf::FzPoint start, mupdf::FzPoint end);
    utils::FzPointPair getPositionsForWordSelection(mupdf::FzPoint begin,
                                                    mupdf::FzPoint end);
    utils::FzPointPair getPositionsForLineSelection(mupdf::FzPoint point);
    std::string getTextFromSelection(mupdf::FzPoint start, mupdf::FzPoint end);

private:
    void setupDisplayList(const mupdf::FzRect& boundPage);
    void setupTextPage(int pageNumber);
    void setupSymbolBounds();
    void setupLinks();
    mupdf::FzPixmap getEmptyPixmap(const mupdf::FzMatrix& scaleMatrix) const;
    void setPageOffsets(int xOffset, int yOffset);

    const mupdf::FzDocument* m_document;
    std::unique_ptr<mupdf::FzPage> m_page;
    std::unique_ptr<mupdf::FzStextPage> m_textPage;
    mupdf::FzDisplayList m_displayList;
    utils::TextSelector m_textSelector;
    QList<mupdf::FzQuad> m_bufferedSelectionRects;
    QList<mupdf::FzLink> m_links;
    std::vector<fz_rect> m_symbolBounds;
    bool m_invertColor = false;
    int m_pageXOffset = 0;
    int m_pageYOffset = 0;
};

}  // namespace application::core
