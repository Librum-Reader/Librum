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

void Document::search(const QString& text)
{
    for(int i = 0; i < m_document.fz_count_pages(); ++i)
    {
        mupdf::FzStextOptions options;
        mupdf::FzStextPage textPage(*this->internal(), i, options);
        const int maxHits = 100;
        int hitMarks[maxHits];
        auto hits = textPage.search_stext_page(text.toStdString().c_str(),
                                               hitMarks, maxHits);

        m_searchHits.reserve(hits.size());
        for(auto& hit : hits)
        {
            m_searchHits.emplace_back(i, fzQuadToQRectF(hit));
        }
    }
}

QRectF Document::fzQuadToQRectF(const mupdf::FzQuad& rect)
{
    float width = rect.ur.x - rect.ul.x;
    float height = rect.ll.y - rect.ul.y;
    return QRectF(rect.ul.x, rect.ul.y, width, height);
}

std::vector<SearchHit>& Document::getSearchHits()
{
    return m_searchHits;
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
