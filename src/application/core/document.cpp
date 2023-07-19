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
    return getDocumentInfo("info:Title");
}

QString Document::getAuthors()
{
    return getDocumentInfo("info:Author");
}

QString Document::getFormat()
{
    return getDocumentInfo("format");
}

QString Document::getCreator()
{
    return getDocumentInfo("info:Creator");
}

QString Document::getCreationDate()
{
    return getDocumentInfo("info:CreationDate");
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

QString Document::getDocumentInfo(const char* key)
{
    try
    {
        auto result = m_document.fz_lookup_metadata2(key);
        return QString::fromStdString(result);
    }
    catch(...)
    {
        return "";
    }
}

}  // namespace application::core