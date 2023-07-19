#include "document.hpp"
#include "page.hpp"
#include "toc/toc_model.hpp"

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
    try
    {
        return QString::fromStdString(
            m_document.fz_lookup_metadata2("info:Title"));
    }
    catch(...)
    {
        return "";
    }
}

QString Document::getAuthors()
{
    try
    {
        return QString::fromStdString(
            m_document.fz_lookup_metadata2("info:Author"));
    }
    catch(...)
    {
        return "";
    }
}

QString Document::getFormat()
{
    try
    {
        return QString::fromStdString(m_document.fz_lookup_metadata2("format"));
    }
    catch(...)
    {
        return "";
    }
}

QString Document::getCreator()
{
    try
    {
        return QString::fromStdString(
            m_document.fz_lookup_metadata2("info:Creator"));
    }
    catch(...)
    {
        return "";
    }
}

QString Document::getCreationDate()
{
    try
    {
        return QString::fromStdString(
            m_document.fz_lookup_metadata2("info:CreationDate"));
    }
    catch(...)
    {
        return "";
    }
}

QImage Document::getCover()
{
    try
    {
        Page page(this, 0);
        return page.renderPage();
    }
    catch(...)
    {
        return QImage();
    }
}

FilteredTOCModel* Document::getFilteredTOCModel()
{
    if(m_TOCModel == nullptr)
    {
        auto data = m_document.fz_load_outline();
        m_TOCModel = std::make_unique<TOCModel>(data.m_internal, m_document);
        m_filteredTOCModel = std::make_unique<FilteredTOCModel>();
        m_filteredTOCModel->setSourceModel(m_TOCModel.get());
    }

    return m_filteredTOCModel.get();
}

const mupdf::FzDocument* Document::internal() const
{
    return &m_document;
}

}  // namespace application::core