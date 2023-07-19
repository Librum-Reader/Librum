#pragma once
#include <QImage>
#include "document.hpp"
#include "mupdf/classes.h"

namespace application::core
{

class Page
{
public:
    Page(const Document* document, int pageNumber);

    QImage renderPage(bool invertColor);
    int getWidth() const;
    int getHeight() const;
    void setZoom(float newZoom);

private:
    mupdf::FzPixmap getEmptyPixmap() const;
    QImage imageFromPixmap(mupdf::FzPixmap pixmap);

    const mupdf::FzDocument* m_document;
    mupdf::FzPage m_page;
    mupdf::FzDisplayList m_displayList;
    mupdf::FzMatrix m_matrix;
};

}  // namespace application::core