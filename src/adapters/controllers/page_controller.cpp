#include "page_controller.hpp"
#include "mupdf/classes.h"

namespace adapters::controllers
{

PageController::PageController(mupdf::FzDocument* document, int pageNumber) :
    m_pageGenerator(document, pageNumber)
{
}

int PageController::getWidth()
{
    return m_pageGenerator.getWidth();
}

int PageController::getHeight()
{
    return m_pageGenerator.getHeight();
}

float PageController::getZoom()
{
    return m_pageGenerator.getZoom();
}

void PageController::setZoom(float newZoom)
{
    m_pageGenerator.setZoom(newZoom);
}

QImage PageController::renderPage()
{
    return m_pageGenerator.renderPage();
}

bool PageController::pointIsAboveText(const QPoint& point)
{
    return m_pageGenerator.pointIsAboveText(point);
}

bool PageController::pointIsAboveLink(const QPoint& point)
{
    return m_pageGenerator.pointIsAboveLink(point);
}

const char* PageController::getLinkUriAtPoint(const QPoint& point)
{
    return m_pageGenerator.getLinkAtPoint(point).uri();
}

QList<QRectF>& PageController::getBufferedSelectionRects()
{
    return m_pageGenerator.getBufferedSelectionRects();
}

void PageController::setInvertColor(bool newInvertColor)
{
    m_pageGenerator.setInvertColor(newInvertColor);
}

void PageController::generateSelectionRects(QPointF start, QPointF end)
{
    m_pageGenerator.getBufferedSelectionRects().clear();
    return m_pageGenerator.generateSelectionRects(start, end);
}

QPair<QPointF, QPointF> PageController::getPositionsForWordSelection(
    QPointF begin, QPointF end)
{
    return m_pageGenerator.getPositionsForWordSelection(begin, end);
}

QPair<QPointF, QPointF> PageController::getPositionsForLineSelection(
    QPointF point)
{
    return m_pageGenerator.getPositionsForLineSelection(point);
}

QString PageController::getTextFromSelection(const QPointF& start,
                                             const QPointF& end)
{
    return m_pageGenerator.getTextFromSelection(start, end);
}

}  // namespace adapters::controllers