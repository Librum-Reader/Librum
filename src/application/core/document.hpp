#pragma once
#include <QImage>
#include <QRectF>
#include <QString>
#include <memory>
#include <vector>
#include "application_export.hpp"
#include "mupdf/classes.h"
#include "toc/filtered_toc_model.hpp"
#include "toc/toc_model.hpp"

namespace application::core
{

struct SearchHit
{
    int pageNumber;
    QRectF rect;
};

class APPLICATION_LIBRARY Document
{
public:
    Document(const QString& filePath);

    int getPageCount() const;
    QString getTitle();
    QString getAuthors();
    QString getFormat();
    QString getCreator();
    QString getCreationDate();
    QImage getCover();
    FilteredTOCModel* getFilteredTOCModel();

    void search(const QString& text);
    std::vector<SearchHit>& getSearchHits();

    const mupdf::FzDocument* internal() const;

private:
    QString getDocumentInfo(const char* key);
    QRectF fzQuadToQRectF(const mupdf::FzQuad& rect);

    std::vector<SearchHit> m_searchHits;
    mupdf::FzDocument m_document;
    std::unique_ptr<TOCModel> m_TOCModel;
    std::unique_ptr<FilteredTOCModel> m_filteredTOCModel;
};

}  // namespace application::core
