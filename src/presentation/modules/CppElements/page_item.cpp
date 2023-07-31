#include "page_item.hpp"
#include <QClipboard>
#include <QDebug>
#include <QImage>
#include <QObject>
#include <QPainter>
#include <QQuickWindow>
#include <QRectF>
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

    m_page->setZoom(m_document->getZoom());
    emit implicitWidthChanged();
    emit implicitHeightChanged();
    update();

    connect(m_document, &DocumentItem::zoomChanged, this,
            &PageItem::updateZoom);

    connect(m_document, &DocumentItem::highlightText, this,
            [this](int pageNumber, QRectF rect)
            {
                if(pageNumber != m_currentPage)
                    return;

                selectPosition(rect);
            });
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

    // Update selection positions to match new zoom
    if(!m_selectionStart.isNull() && !m_selectionEnd.isNull())
    {
        m_selectionStart =
            m_page->scalePointToCurrentZoom(m_selectionStart, oldZoom);
        m_selectionEnd =
            m_page->scalePointToCurrentZoom(m_selectionEnd, oldZoom);
        generateSelection();
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

    paintSelectionOnPage(painter);

    n->setTexture(window()->createTextureFromImage(image));
    n->setRect(boundingRect());
    return n;
}

void PageItem::paintSelectionOnPage(QPainter& painter)
{
    auto& bufferedSelectionRects = m_page->getBufferedSelectionRects();
    for(auto rect : bufferedSelectionRects)
    {
        QColor selectionColor(134, 171, 175, 125);
        painter.setCompositionMode(QPainter::CompositionMode_Multiply);
        painter.fillRect(rect, selectionColor);
    }
}

void PageItem::select(int beginX, int beginY, int endX, int endY)
{
    m_selectionStart = QPointF(beginX, beginY);
    m_selectionEnd = QPointF(endX, endY);

    generateSelection();
    update();
}

void PageItem::selectSingleWord(int x, int y)
{
    QPointF wordPos = QPointF(x, y);
    auto points = m_page->getPositionsForWordSelection(wordPos, wordPos);

    m_selectionStart = points.first;
    m_selectionEnd = points.second;

    generateSelection();
    update();
}

void PageItem::selectMultipleWords(int beginX, int beginY, int endX, int endY)
{
    QPointF begin(beginX, beginY);
    QPointF end(endX, endY);

    auto positions = m_page->getPositionsForWordSelection(begin, end);
    m_selectionStart = positions.first;
    m_selectionEnd = positions.second;

    generateSelection();
    update();
}

void PageItem::removeSelection()
{
    m_page->getBufferedSelectionRects().clear();
    update();
}

void PageItem::copySelectedText()
{
    auto clipboard = QApplication::clipboard();
    QString text =
        m_page->getTextFromSelection(m_selectionStart, m_selectionEnd);

    clipboard->setText(text);
}

bool PageItem::pointIsAboveText(int x, int y)
{
    return m_page->pointIsAboveText(QPoint(x, y));
}

void PageItem::generateSelection()
{
    m_page->getBufferedSelectionRects().clear();
    m_page->generateSelectionRects(m_selectionStart, m_selectionEnd);
}

void PageItem::selectPosition(QRectF rect)
{
    mupdf::FzPoint leftMiddle(rect.left(), rect.center().y());
    mupdf::FzPoint rightMiddle(rect.right(), rect.center().y());

    // Make sure to apply the current zoom to the points since "select" expects
    // them to be in the current zoom.
    auto zoom = m_page->getZoom();
    mupdf::FzMatrix matrix;
    matrix = matrix.fz_scale(zoom, zoom);

    leftMiddle = leftMiddle.fz_transform_point(matrix);
    rightMiddle = rightMiddle.fz_transform_point(matrix);

    select(leftMiddle.x, leftMiddle.y, rightMiddle.x, rightMiddle.y);
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