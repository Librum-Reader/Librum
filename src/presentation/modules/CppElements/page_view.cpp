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
#include "highlight.hpp"
#include "page_controller.hpp"
#include "qnamespace.h"

using adapters::controllers::BookController;
using adapters::controllers::PageController;
using domain::entities::Highlight;
using namespace application::core;

namespace cpp_elements
{

cpp_elements::PageView::PageView()
{
    setFlag(QQuickItem::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);

    m_tripleClickTimer.setInterval(400);
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

    connect(m_bookController, &BookController::selectText, this,
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
    paintHighlightsOnPage(painter);

    n->setTexture(window()->createTextureFromImage(image));
    n->setRect(boundingRect());
    return n;
}

void PageView::mouseDoubleClickEvent(QMouseEvent* event)
{
    if(event->button() == Qt::RightButton)
        return;

    m_selectionStart = QPointF(event->position().x(), event->position().y());
    m_selectionEnd = QPointF(event->position().x(), event->position().y());

    selectSingleWord();

    m_tripleClickTimer.start();
    m_doubleClickHold = true;
}

void PageView::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::RightButton)
        return;

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

    if(m_startedMousePressOnLink &&
       m_pageController->pointIsAboveLink(mousePoint))
    {
        auto uri = m_pageController->getLinkUriAtPoint(mousePoint);
        m_bookController->followLink(uri);
    }
    m_startedMousePressOnLink = false;

    // This gets triggered when the user simply clicks on the page, without
    // dragging the mouse, so on a normal click. In this case we want to
    // reset the selection.
    if(m_selectionStart == QPointF(mouseX, mouseY))
    {
        removeSelection();
    }
    else
    {
        emitSelectionFinishedSignal();
    }

    m_doubleClickHold = false;
}

void PageView::mouseMoveEvent(QMouseEvent* event)
{
    if(event->buttons() == Qt::RightButton)
        return;

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
    if(m_disableHoverEvents)
        return;

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
    auto& selectionRects = m_pageController->getBufferedSelectionRects();
    for(auto rect : selectionRects)
    {
        QColor selectionColor(134, 171, 175, 125);
        painter.setCompositionMode(QPainter::CompositionMode_Multiply);
        painter.fillRect(rect, selectionColor);
    }
}

void PageView::emitSelectionFinishedSignal()
{
    float mostLeft = 9999999;  // Very big default
    float mostRight = 0;
    float topY = 9999999;
    for(auto& selectionRect : m_pageController->getBufferedSelectionRects())
    {
        if(selectionRect.x() < mostLeft)
            mostLeft = selectionRect.x();

        if(selectionRect.x() + selectionRect.width() > mostRight)
            mostRight = selectionRect.x() + selectionRect.width();

        if(selectionRect.top() < topY)
            topY = selectionRect.top();
    }

    auto centerX = (mostLeft + mostRight) / 2;
    emit m_bookController->textSelectionFinished(centerX, topY);
}

void PageView::paintHighlightsOnPage(QPainter& painter)
{
    for(auto& highlight : m_bookController->getHighlights())
    {
        if(highlight.getPageNumber() != m_pageNumber)
            continue;

        for(auto rect : highlight.getRects())
        {
            // We store the highlights zoom independent, so we need to scale
            // them to the current zoom here.
            auto qRect = rect.getQRect();
            utils::scaleQRectFToZoom(qRect, m_pageController->getZoom());
            rect.setQRect(qRect);

            painter.setCompositionMode(QPainter::CompositionMode_Multiply);
            painter.fillRect(rect.getQRect(), highlight.getColor());
        }
    }
}

void PageView::removeConflictingHighlights(Highlight& highlight)
{
    bool existingHighlightRemoved = false;

    auto& highlights = m_bookController->getHighlights();
    for(int i = 0; i < highlights.size(); ++i)
    {
        auto& existingHighlight = highlights[i];
        if(existingHighlight.getPageNumber() != highlight.getPageNumber())
            continue;

        for(int u = 0; u < highlight.getRects().size(); ++u)
        {
            auto rect = highlight.getRects()[u].getQRect();

            for(int k = 0; k < existingHighlight.getRects().size(); ++k)
            {
                auto existingRect = existingHighlight.getRects()[k].getQRect();

                // New rect intersects with old rect
                if(rect.intersects(existingRect))
                {
                    // Make sure that the rects are on the same line. Depending
                    // on the line height, lines above eachother will overlap,
                    // but its only an intersect when they are on the same line.
                    auto shorterRect = rect.height() <= existingRect.height()
                                           ? rect
                                           : existingRect;
                    auto intersectH = rect.intersected(existingRect).height();
                    bool onSameLine = intersectH >= shorterRect.height() * 0.75;

                    if(onSameLine)
                    {
                        auto uuid = highlights[i].getUuid();
                        m_bookController->removeHighlight(uuid);
                        --i;
                        existingHighlightRemoved = true;
                        break;
                    }
                }
            }

            if(existingHighlightRemoved)
            {
                existingHighlightRemoved = false;
                break;
            }
        }
    }
}

bool PageView::mouseAboveSelection(const QPointF mouse)
{
    auto& selectionRects = m_pageController->getBufferedSelectionRects();
    for(auto& rect : selectionRects)
    {
        if(rect.contains(mouse))
            return true;
    }

    return false;
}

void PageView::createHighlightFromCurrentSelection()
{
    auto bufferedSelectionRects = m_pageController->getBufferedSelectionRects();

    // Make sure to restore the rects to their original size, since we want to
    // store them, so they need to be zoom independent.
    for(auto& rect : bufferedSelectionRects)
    {
        utils::restoreQRect(rect, m_pageController->getZoom());
    }

    removeSelection();

    static int i = 0;
    ++i;

    auto pageNumber = m_pageNumber;
    auto color = QColor(i % 2 == 0 ? 255 : 0, i % 2 == 0 ? 0 : 255, 0, 125);
    auto rects = bufferedSelectionRects;
    Highlight highlight(pageNumber, color);
    highlight.setRects(rects);

    removeConflictingHighlights(highlight);
    m_bookController->addHighlight(highlight);
    m_bookController->saveHighlights();

    update();
}

void PageView::setPointingCursor()
{
    if(QApplication::overrideCursor() == nullptr ||
       *QApplication::overrideCursor() != Qt::PointingHandCursor)
    {
        resetCursorToDefault();
        QApplication::setOverrideCursor(Qt::PointingHandCursor);
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
        setPointingCursor();
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

bool PageView::disableHoverEvents() const
{
    return m_disableHoverEvents;
}

void PageView::setDisableHoverEvents(bool newDisableHoverEvents)
{
    m_disableHoverEvents = newDisableHoverEvents;
}

}  // namespace cpp_elements