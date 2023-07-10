#pragma once
#include <QString>
#include "mupdf/classes.h"

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

    const mupdf::FzDocument* internal() const;

private:
    mupdf::FzDocument m_document;
};

}  // namespace application::core