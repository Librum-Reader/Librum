#include "document.hpp"

namespace application::core
{

Document::Document(const QString& filePath) :
    m_document(filePath.toStdString().c_str())
{
}

int Document::pageCount() const
{
    return m_document.fz_count_pages();
}

const mupdf::FzDocument* Document::internal() const
{
    return &m_document;
}

}  // namespace application::core