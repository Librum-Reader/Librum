#include "page_item.hpp"
#include <QObject>
#include <QQuickWindow>
#include <QSGSimpleTextureNode>

namespace cpp_elements
{

int PageItem::getImplicitWidth() const
{
    return m_page->getWidth();
}

int PageItem::getImplicitHeight() const
{
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
}

int PageItem::getCurrentPage() const
{
    return m_currentPage;
}

void PageItem::setCurrentPage(int newCurrentPage)
{
    m_currentPage = newCurrentPage;
}

void PageItem::geometryChanged(const QRectF& newGeometry,
                               const QRectF& oldGeometry)
{
    if(newGeometry.size().isEmpty())
        return;

    if(newGeometry.width() != oldGeometry.width() ||
       newGeometry.height() != newGeometry.height())
    {
        qDebug() << "New";
        update();
    }

    QQuickItem::geometryChanged(newGeometry, oldGeometry);
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