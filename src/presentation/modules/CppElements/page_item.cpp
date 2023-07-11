#include "page_item.hpp"
#include <QDebug>
#include <QObject>
#include <QQuickWindow>
#include <QSGSimpleTextureNode>

namespace cpp_elements
{

cpp_elements::PageItem::PageItem()
{
    setFlag(QQuickItem::ItemHasContents, true);
}

int PageItem::getImplicitWidth() const
{
    if(m_document == nullptr)
        return 0;

    return m_page->getWidth();
}

int PageItem::getImplicitHeight() const
{
    if(m_document == nullptr)
        return 0;

    return m_page->getHeight();
}

DocumentItem* PageItem::getDocument() const
{
    return m_document;
}

void PageItem::setDocument(DocumentItem* newDocument)
{
    m_document = newDocument;
    m_page = std::make_unique<application::core::Page>(m_document->internal(),
                                                       m_currentPage);

    connect(m_document, &DocumentItem::zoomChanged, this,
            &PageItem::updateZoom);

    m_page->setZoom(m_document->getZoom());
    emit implicitWidthChanged();
    emit implicitHeightChanged();
    update();
}

int PageItem::getPageNumber() const
{
    return m_currentPage;
}

void PageItem::setPageNumber(int newCurrentPage)
{
    m_currentPage = newCurrentPage;
}

void PageItem::updateZoom(float newZoom)
{
    m_page->setZoom(newZoom);
    emit implicitWidthChanged();
    emit implicitHeightChanged();
}

void PageItem::geometryChange(const QRectF& newGeometry,
                              const QRectF& oldGeometry)
{
    if(newGeometry.size().isEmpty())
        return;

    if(newGeometry.width() != oldGeometry.width() ||
       newGeometry.height() != newGeometry.height())
    {
        update();
    }

    QQuickItem::geometryChange(newGeometry, oldGeometry);
}

QSGNode* PageItem::updatePaintNode(QSGNode* node, UpdatePaintNodeData* nodeData)
{
    Q_UNUSED(nodeData);
    QSGSimpleTextureNode* n = static_cast<QSGSimpleTextureNode*>(node);
    if(!n)
    {
        n = new QSGSimpleTextureNode();
        n->setOwnsTexture(true);
    }

    auto image = m_page->renderPage();
    n->setTexture(window()->createTextureFromImage(image));
    n->setRect(boundingRect());
    return n;
}

}  // namespace cpp_elements