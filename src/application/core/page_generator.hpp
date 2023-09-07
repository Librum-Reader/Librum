#pragma once
#include <QList>
#include <QPair>
#include <string>
#include <vector>
#include "application_export.hpp"
#include "mupdf/classes.h"
#include "text_selector.hpp"

namespace application::core
{

class APPLICATION_EXPORT PageGenerator
{
public:
    PageGenerator(mupdf::FzDocument* document, int pageNumber);

    int getWidth() const;
    int getHeight() const;

    float getZoom() const;
    void setZoom(float newZoom);

    void setInvertColor(bool newInvertColor);

    mupdf::FzPixmap renderPage();

    bool pointIsAboveText(const mupdf::FzPoint& point);
    bool pointIsAboveLink(const mupdf::FzPoint& point);
    mupdf::FzLink getLinkAtPoint(const mupdf::FzPoint& point);

    QList<mupdf::FzQuad>& getBufferedSelectionRects();
    void generateSelectionRects(mupdf::FzPoint start, mupdf::FzPoint end);
    QPair<mupdf::FzPoint, mupdf::FzPoint> getPositionsForWordSelection(
        mupdf::FzPoint begin, mupdf::FzPoint end);
    QPair<mupdf::FzPoint, mupdf::FzPoint> getPositionsForLineSelection(
        mupdf::FzPoint point);
    std::string getTextFromSelection(const mupdf::FzPoint& start,
                                     const mupdf::FzPoint& end);

private:
    void setupDisplayList(const mupdf::FzRect& boundPage);
    void setupTextPage(int pageNumber);
    void setupSymbolBounds();
    void setupLinks();
    mupdf::FzPixmap getEmptyPixmap() const;

    const mupdf::FzDocument* m_document;
    std::unique_ptr<mupdf::FzPage> m_page;
    std::unique_ptr<mupdf::FzStextPage> m_textPage;
    mupdf::FzDisplayList m_displayList;
    mupdf::FzMatrix m_matrix;
    utils::TextSelector m_textSelector;
    QList<mupdf::FzQuad> m_bufferedSelectionRects;
    QList<mupdf::FzLink> m_links;
    std::vector<fz_rect> m_symbolBounds;
    bool m_invertColor = false;

    bool m_pixmapOutdated = true;
    mupdf::FzPixmap m_pixmap;
};

}  // namespace application::core
