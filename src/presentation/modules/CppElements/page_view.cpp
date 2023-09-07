#include "page_view.hpp"
#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QImage>
#include <QObject>
#include <QPainter>
#include <QQuickItem>
#include <QQuickWindow>
#include <QRectF>
#include <QSGSimpleTextureNode>
#include <QtWidgets/QApplication>
#include "fz_utils.hpp"
#include "page_controller.hpp"
#include "qnamespace.h"

using adapters::controllers::BookController;
using adapters::controllers::PageController;
using namespace application::core;

namespace cpp_elements
{

cpp_elements::PageView::PageView()
{
    setFlag(QQuickItem::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);

    m_tripleClickTimer.setInterval(450);
    m_tripleClickTimer.setSingleShot(true);
}

void PageView::setBookController(BookController* newBookController)
{
    m_bookController = newBookController;

    m_pageController = std::make_unique<PageController>(
        m_bookController->getFzDocument(), m_pageNumber);
    m_pageController->setZoom(m_bookController->getZoom());

    // Setup connections to the BookController
    connect(m_bookController, &BookController::zoomChanged, this,
            &PageView::updateZoom);

    connect(m_bookController, &BookController::highlightText, this,
            [this](int pageNumber, QPointF left, QPointF right)
            {
                if(pageNumber != m_pageNumber)
                    return;

                m_selectionStart = QPointF(left.x(), left.y());
                m_selectionEnd = QPointF(right.x(), right.y());

                createSelection();
            });
}

int PageView::getImplicitWidth() const
{
    return m_pageController->getWidth();
}

int PageView::getImplicitHeight() const
{
    return m_pageController->getHeight();
}

int PageView::getPageNumber() const
{
    return m_pageNumber;
}

void PageView::setPageNumber(int newCurrentPage)
{
    m_pageNumber = newCurrentPage;
}

void PageView::updateZoom(float newZoom)
{
    auto oldZoom = m_pageController->getZoom();
    m_pageController->setZoom(newZoom);

    // Update selection positions to match new zoom
    if(!m_selectionStart.isNull() && !m_selectionEnd.isNull())
    {
        m_selectionStart =
            utils::scalePointToCurrentZoom(m_selectionStart, oldZoom, newZoom);
        m_selectionEnd =
            utils::scalePointToCurrentZoom(m_selectionEnd, oldZoom, newZoom);

        m_pageController->generateSelectionRects(m_selectionStart,
                                                 m_selectionEnd);
    }

    emit implicitWidthChanged();
    emit implicitHeightChanged();
}

void PageView::geometryChange(const QRectF& newGeometry,
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

QSGNode* PageView::updatePaintNode(QSGNode* node, UpdatePaintNodeData* nodeData)
{
    Q_UNUSED(nodeData);
    QSGSimpleTextureNode* n = static_cast<QSGSimpleTextureNode*>(node);
    if(!n)
    {
        n = new QSGSimpleTextureNode();
        n->setOwnsTexture(true);
    }

    auto image = m_pageController->renderPage();
    QPainter painter(&image);

    paintSelectionOnPage(painter);

    n->setTexture(window()->createTextureFromImage(image));
    n->setRect(boundingRect());
    return n;
}

void PageView::mouseDoubleClickEvent(QMouseEvent* event)
{
    m_selectionStart = QPointF(event->position().x(), event->position().y());
    m_selectionEnd = QPointF(event->position().x(), event->position().y());
    selectSingleWord();

    m_tripleClickTimer.start();
    m_doubleClickHold = true;
}

void PageView::mousePressEvent(QMouseEvent* event)
{
    int mouseX = event->position().x();
    int mouseY = event->position().y();
    QPoint mousePoint(mouseX, mouseY);

    forceActiveFocus();

    if(m_pageController->pointIsAboveLink(mousePoint))
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

void PageView::mouseReleaseEvent(QMouseEvent* event)
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

    if(m_startedMousePressOnLink &&
       m_pageController->pointIsAboveLink(mousePoint))
    {
        auto uri = m_pageController->getLinkUriAtPoint(mousePoint);
        m_bookController->followLink(uri);
    }
    m_startedMousePressOnLink = false;

    m_doubleClickHold = false;
}

void PageView::mouseMoveEvent(QMouseEvent* event)
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
        createSelection();
}

void PageView::hoverMoveEvent(QHoverEvent* event)
{
    int mouseX = event->position().x();
    int mouseY = event->position().y();

    setCorrectCursor(mouseX, mouseY);
}

void PageView::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_C && event->modifiers() == Qt::ControlModifier)
    {
        copySelectedText();
    }
}

void PageView::paintSelectionOnPage(QPainter& painter)
{
    auto bufferedSelectionRects = m_pageController->getBufferedSelectionRects();
    for(auto rect : bufferedSelectionRects)
    {
        QColor selectionColor(134, 171, 175, 125);
        painter.setCompositionMode(QPainter::CompositionMode_Multiply);
        painter.fillRect(rect, selectionColor);
    }
}

void PageView::createSelection()
{
    m_pageController->generateSelectionRects(m_selectionStart, m_selectionEnd);
    update();
}

void PageView::removeSelection()
{
    m_pageController->clearBufferedSelectionRects();
    update();

    m_selectionStart = QPointF(0, 0);
    m_selectionEnd = QPointF(0, 0);
}

void PageView::selectSingleWord()
{
    auto points = m_pageController->getPositionsForWordSelection(
        m_selectionStart, m_selectionEnd);

    m_selectionStart = points.first;
    m_selectionEnd = points.second;

    createSelection();
}

void PageView::selectMultipleWords()
{
    auto positions = m_pageController->getPositionsForWordSelection(
        m_selectionStart, m_selectionEnd);
    m_selectionStart = positions.first;
    m_selectionEnd = positions.second;

    createSelection();
}

void PageView::selectLine()
{
    auto positions =
        m_pageController->getPositionsForLineSelection(m_selectionStart);
    m_selectionStart = positions.first;
    m_selectionEnd = positions.second;

    createSelection();
}

void PageView::copySelectedText()
{
    auto clipboard = QApplication::clipboard();
    QString text = m_pageController->getTextFromSelection(m_selectionStart,
                                                          m_selectionEnd);

    clipboard->setText(text);
}

void PageView::resetCursorToDefault()
{
    while(QApplication::overrideCursor() != nullptr &&
          *QApplication::overrideCursor() != Qt::ArrowCursor)
    {
        QApplication::restoreOverrideCursor();
    }
}

void PageView::setCorrectCursor(int x, int y)
{
    if(m_pageController->pointIsAboveLink(QPoint(x, y)))
    {
        if(QApplication::overrideCursor() == nullptr ||
           *QApplication::overrideCursor() != Qt::PointingHandCursor)
        {
            resetCursorToDefault();
            QApplication::setOverrideCursor(Qt::PointingHandCursor);
        }
    }
    else if(m_pageController->pointIsAboveText(QPoint(x, y)))
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

void PageView::setColorInverted(bool newColorInverted)
{
    // This method gets called on initialisation of the page item, but we don't
    // want to redraw it then, so we skip it if it's called for the first time.
    m_pageController->setInvertColor(newColorInverted);
    if(!m_firstTimeColorInverted)
        update();

    m_firstTimeColorInverted = false;
}

}  // namespace cpp_elements