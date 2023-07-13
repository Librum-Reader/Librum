#pragma once
#include <QImage>
#include <QString>
#include <memory>
#include "mupdf/classes.h"
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
    TOCModel* getTOCModel();

    const mupdf::FzDocument* internal() const;

private:
    mupdf::FzDocument m_document;
    std::unique_ptr<TOCModel> m_TOCModel;
};

}  // namespace application::core