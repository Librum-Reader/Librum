#include "page_item.hpp"
#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QImage>
#include <QObject>
#include <QPainter>
#include <QQuickWindow>
#include <QRectF>
#include <QSGSimpleTextureNode>
#include <QtWidgets/QApplication>
#include "qnamespace.h"

namespace cpp_elements
{

cpp_elements::PageItem::PageItem()
{
    setFlag(QQuickItem::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);

    m_tripleClickTimer.setInterval(450);
    m_tripleClickTimer.setSingleShot(true);
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

void PageItem::mouseDoubleClickEvent(QMouseEvent* event)
{
    m_selectionStart = QPointF(event->position().x(), event->position().y());
    m_selectionEnd = QPointF(event->position().x(), event->position().y());
    selectSingleWord();

    m_tripleClickTimer.start();
    m_doubleClickHold = true;
}

void PageItem::mousePressEvent(QMouseEvent* event)
{
    int mouseX = event->position().x();
    int mouseY = event->position().y();
    QPoint mousePoint(mouseX, mouseY);

    forceActiveFocus();
    removeSelection();

    if(m_page->pointIsAboveLink(mousePoint))
        m_startedMousePressOnLink = true;

    // Select line when left mouse button is pressed 3 times
    if(m_tripleClickTimer.isActive())
    {
        m_selectionStart = mousePoint;
        selectLine();
        m_tripleClickTimer.stop();
        return;
    }

    m_selectionStart = mousePoint;
}

void PageItem::mouseReleaseEvent(QMouseEvent* event)
{
    int mouseX = event->position().x();
    int mouseY = event->position().y();
    QPoint mousePoint(mouseX, mouseY);

    // This gets triggered when the user simply clicks on the page, without
    // dragging the mouse, so on a normal click. In this case we want to reset
    // the highlight.
    if(m_selectionStart == QPointF(mouseX, mouseY))
    {
        removeSelection();
    }

    if(m_startedMousePressOnLink && m_page->pointIsAboveLink(mousePoint))
    {
        auto link = m_page->getLinkAtPoint(mousePoint);
        followLink(link);
    }
    m_startedMousePressOnLink = false;

    m_doubleClickHold = false;
}

void PageItem::mouseMoveEvent(QMouseEvent* event)
{
    int mouseX = event->position().x();
    int mouseY = event->position().y();

    // 'hoverMoveEvent' is not triggered when the left mouse button is pressed,
    // thus the cursor will not change correctly. Make sure to handle it here.
    setCorrectCursor(mouseX, mouseY);

    m_selectionEnd = QPointF(mouseX, mouseY);
    if(m_doubleClickHold)
        selectMultipleWords();
    else
        drawSelection();
}

void PageItem::hoverMoveEvent(QHoverEvent* event)
{
    int mouseX = event->position().x();
    int mouseY = event->position().y();

    setCorrectCursor(mouseX, mouseY);
}

void PageItem::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_C && event->modifiers() == Qt::ControlModifier)
    {
        copySelectedText();
    }
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

void PageItem::drawSelection()
{
    generateSelection();
    update();
}

void PageItem::generateSelection()
{
    m_page->getBufferedSelectionRects().clear();
    m_page->generateSelectionRects(m_selectionStart, m_selectionEnd);
}

void PageItem::removeSelection()
{
    m_page->getBufferedSelectionRects().clear();
    update();

    m_selectionStart = QPointF(0, 0);
    m_selectionEnd = QPointF(0, 0);
}

void PageItem::selectSingleWord()
{
    auto points =
        m_page->getPositionsForWordSelection(m_selectionStart, m_selectionEnd);

    m_selectionStart = points.first;
    m_selectionEnd = points.second;

    drawSelection();
}

void PageItem::selectMultipleWords()
{
    auto positions =
        m_page->getPositionsForWordSelection(m_selectionStart, m_selectionEnd);
    m_selectionStart = positions.first;
    m_selectionEnd = positions.second;

    drawSelection();
}

void PageItem::selectLine()
{
    auto positions = m_page->getPositionsForLineSelection(m_selectionStart);
    m_selectionStart = positions.first;
    m_selectionEnd = positions.second;

    drawSelection();
}

void PageItem::copySelectedText()
{
    auto clipboard = QApplication::clipboard();
    QString text =
        m_page->getTextFromSelection(m_selectionStart, m_selectionEnd);

    clipboard->setText(text);
}

void PageItem::resetCursorToDefault()
{
    while(QApplication::overrideCursor() != nullptr &&
          *QApplication::overrideCursor() != Qt::ArrowCursor)
    {
        QApplication::restoreOverrideCursor();
    }
}

void PageItem::setCorrectCursor(int x, int y)
{
    if(m_page->pointIsAboveLink(QPoint(x, y)))
    {
        if(QApplication::overrideCursor() == nullptr ||
           *QApplication::overrideCursor() != Qt::PointingHandCursor)
        {
            resetCursorToDefault();
            QApplication::setOverrideCursor(Qt::PointingHandCursor);
        }
    }
    else if(m_page->pointIsAboveText(QPoint(x, y)))
    {
        if(QApplication::overrideCursor() == nullptr ||
           *QApplication::overrideCursor() != Qt::IBeamCursor)
        {
            resetCursorToDefault();
            QApplication::setOverrideCursor(Qt::IBeamCursor);
        }
    }
    else
    {
        resetCursorToDefault();
    }
}

void PageItem::followLink(mupdf::FzLink& link)
{
    auto uri = link.uri();

    if(mupdf::fz_is_external_link(uri))
    {
        QDesktopServices::openUrl(QUrl(uri));
    }
    else
    {
        auto fzDocument = m_document->internal()->internal();
        float yp = 0;

        auto location = fzDocument->fz_resolve_link(uri, nullptr, &yp);
        int pageNumber = fzDocument->fz_page_number_from_location(location);

        emit m_document->goToPosition(pageNumber, yp);
    }
}

void PageItem::selectPosition(QRectF rect)
{
    mupdf::FzPoint leftMiddle(rect.left(), rect.center().y());
    mupdf::FzPoint rightMiddle(rect.right(), rect.center().y());

    // Make sure to apply the current zoom to the points since "drawSelection"
    // expects them to be in the current zoom.
    auto zoom = m_page->getZoom();
    mupdf::FzMatrix matrix;
    matrix = matrix.fz_scale(zoom, zoom);

    leftMiddle = leftMiddle.fz_transform_point(matrix);
    rightMiddle = rightMiddle.fz_transform_point(matrix);

    m_selectionStart = QPointF(leftMiddle.x, leftMiddle.y);
    m_selectionEnd = QPointF(rightMiddle.x, rightMiddle.y);

    drawSelection();
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