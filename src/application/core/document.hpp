#pragma once
#include <QImage>
#include <QString>
#include <memory>
#include "mupdf/classes.h"
#include "toc/filtered_toc_model.hpp"
#include "toc/toc_model.hpp"

namespace application::core
{

class Document
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

    const mupdf::FzDocument* internal() const;

private:
    QString getDocumentInfo(const char* key);

    mupdf::FzDocument m_document;
    std::unique_ptr<TOCModel> m_TOCModel;
    std::unique_ptr<FilteredTOCModel> m_filteredTOCModel;
};

}  // namespace application::core