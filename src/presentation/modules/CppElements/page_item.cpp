#include "page_item.hpp"
#include <QClipboard>
#include <QDebug>
#include <QImage>
#include <QObject>
#include <QPainter>
#include <QQuickWindow>
#include <QSGSimpleTextureNode>
#include <QtWidgets/QApplication>

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
    auto oldZoom = m_page->getZoom();
    m_page->setZoom(newZoom);

    // Update highlight positions to match new zoom
    if(!m_highlightStart.isNull() && !m_highlightEnd.isNull())
    {
        m_highlightStart =
            m_page->scalePointToCurrentZoom(m_highlightStart, oldZoom);
        m_highlightEnd =
            m_page->scalePointToCurrentZoom(m_highlightEnd, oldZoom);
        generateHighlights();
    }

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
    QPainter painter(&image);

    paintHighlightsOnPage(painter);

    n->setTexture(window()->createTextureFromImage(image));
    n->setRect(boundingRect());
    return n;
}

void PageItem::paintHighlightsOnPage(QPainter& painter)
{
    auto& bufferedHighlights = m_page->getBufferedHighlights();
    for(auto rect : bufferedHighlights)
    {
        QColor highlightColor(134, 171, 175, 125);
        painter.setCompositionMode(QPainter::CompositionMode_Multiply);
        painter.fillRect(rect, highlightColor);
    }
}

void PageItem::setHighlight(int beginX, int beginY, int endX, int endY)
{
    m_highlightStart = QPointF(beginX, beginY);
    m_highlightEnd = QPointF(endX, endY);

    generateHighlights();
    update();
}

void PageItem::removeHighlight()
{
    m_page->getBufferedHighlights().clear();
    update();
}

void PageItem::copyHighlightedText()
{
    auto clipboard = QApplication::clipboard();
    QString text =
        m_page->getTextFromCurrentHighlight(m_highlightStart, m_highlightEnd);

    clipboard->setText(text);
}

bool PageItem::pointIsAboveText(int x, int y)
{
    return m_page->pointIsAboveText(QPoint(x, y));
}

void PageItem::generateHighlights()
{
    m_page->getBufferedHighlights().clear();
    m_page->setHighlight(m_highlightStart, m_highlightEnd);
}

void PageItem::setColorInverted(bool newColorInverted)
{
    // This method gets called on initialisation of the page item, but we don't
    // want to redraw it then, so we skip it if it's called for the first time.
    m_page->setInvertColor(newColorInverted);
    if(!m_firstTimeColorInverted)
        update();

    m_firstTimeColorInverted = false;
}

}  // namespace cpp_elements