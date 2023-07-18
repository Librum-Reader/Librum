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
    Page page(this, 0);
    return page.renderPage();
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

QString Document::getDocumentInfo(const QString& key)
{
    auto cKey = key.toLatin1().data();

    const int size = 100;
    char dataBuffer[size];
    auto res = m_document.fz_lookup_metadata(cKey, dataBuffer, size);
    if(res == -1)
    {
        return "";
    }
    else if(res > size)
    {
        // Refetch the data with the correct size
        m_document.fz_lookup_metadata(cKey, dataBuffer, res);
    }

    return QString(dataBuffer);
}

}  // namespace application::core