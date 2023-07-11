#include "document.hpp"
#include "page.hpp"

namespace application::core
{

Document::Document(const QString& filePath) :
    m_document(filePath.toStdString().c_str())
{
}

int Document::getPageCount() const
{
    return m_document.fz_count_pages();
}

QString Document::getTitle()
{
    auto x = m_document.fz_lookup_metadata("info:Title");
    return QString::fromStdString(m_document.fz_lookup_metadata("info:Title"));
}

QString Document::getAuthors()
{
    return QString::fromStdString(m_document.fz_lookup_metadata("info:Author"));
}

QString Document::getFormat()
{
    return QString::fromStdString(m_document.fz_lookup_metadata("format"));
}

QString Document::getCreator()
{
    return QString::fromStdString(
        m_document.fz_lookup_metadata("info:Creator"));
}

QString Document::getCreationDate()
{
    return QString::fromStdString(
        m_document.fz_lookup_metadata("info:CreationDate"));
}

QImage Document::getCover()
{
    Page page(this, 0);
    return page.renderPage();
}

const mupdf::FzDocument* Document::internal() const
{
    return &m_document;
}

}  // namespace application::core