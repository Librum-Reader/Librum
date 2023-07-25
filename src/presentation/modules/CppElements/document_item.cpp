#include "document_item.hpp"
#include <QUrl>
#include <memory>
#include "document.hpp"

using application::core::Document;

namespace cpp_elements
{

void DocumentItem::setFilePath(const QString& newFilePath)
{
    m_document = std::make_unique<Document>(QUrl(newFilePath).path());

    emit filePathChanged(newFilePath);
    emit pageCountChanged(m_document->getPageCount());
    emit tableOfContentsChanged();
}

int DocumentItem::getPageCount() const
{
    if(!m_document)
        return 0;

    return m_document->getPageCount();
}

const Document* DocumentItem::internal() const
{
    return m_document.get();
}

void DocumentItem::setCurrentPage(int newCurrentPage)
{
    m_currentPage = newCurrentPage;
    emit currentPageChanged(m_currentPage);
}

int DocumentItem::getCurrentPage() const
{
    return m_currentPage;
}

float DocumentItem::getZoom() const
{
    return m_zoom;
}

void DocumentItem::setZoom(float newZoom)
{
    if(qFuzzyCompare(m_zoom, newZoom))
        return;

    m_zoom = newZoom;
    emit zoomChanged(m_zoom);
}

application::core::FilteredTOCModel* DocumentItem::getTableOfContents() const
{
    if(m_document == nullptr)
        return nullptr;

    return m_document->getFilteredTOCModel();
}

}  // namespace cpp_elements