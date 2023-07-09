#pragma once
#include "mupdf/classes.h"

namespace application::core
{

class Document
{
public:
    Document(const QString& filePath);

    int pageCount() const;
    const mupdf::FzDocument* internal() const;

private:
    mupdf::FzDocument m_document;
};

}  // namespace application::core