#include "page_controller.hpp"
#include "fz_utils.hpp"
#include "mupdf/classes.h"

using namespace application::core;

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
    return utils::qImageFromPixmap(m_pageGenerator.renderPage());
}

bool PageController::pointIsAboveText(const QPointF& point)
{
    auto fzPoint = utils::qPointToFzPoint(point);
    return m_pageGenerator.pointIsAboveText(fzPoint);
}

bool PageController::pointIsAboveLink(const QPointF& point)
{
    auto fzPoint = utils::qPointToFzPoint(point);
    return m_pageGenerator.pointIsAboveLink(fzPoint);
}

const char* PageController::getLinkUriAtPoint(const QPointF& point)
{
    auto fzPoint = utils::qPointToFzPoint(point);
    return m_pageGenerator.getLinkAtPoint(fzPoint).uri();
}

QList<QRectF> PageController::getBufferedSelectionRects()
{
    QList<QRectF> result;

    auto& rects = m_pageGenerator.getBufferedSelectionRects();
    rects.reserve(rects.size());
    for(auto& rect : rects)
    {
        result.append(utils::fzQuadToQRectF(rect));
    }

    return result;
}

void PageController::setInvertColor(bool newInvertColor)
{
    m_pageGenerator.setInvertColor(newInvertColor);
}

void PageController::generateSelectionRects(QPointF start, QPointF end)
{
    m_pageGenerator.getBufferedSelectionRects().clear();
    return m_pageGenerator.generateSelectionRects(utils::qPointToFzPoint(start),
                                                  utils::qPointToFzPoint(end));
}

void PageController::clearBufferedSelectionRects()
{
    m_pageGenerator.getBufferedSelectionRects().clear();
}

QPair<QPointF, QPointF> PageController::getPositionsForWordSelection(
    QPointF start, QPointF end)
{
    auto fzStart = utils::qPointToFzPoint(start);
    auto fzEnd = utils::qPointToFzPoint(end);

    auto res = m_pageGenerator.getPositionsForWordSelection(fzStart, fzEnd);
    return QPointFPair(utils::fzPointToQPoint(res.first),
                       utils::fzPointToQPoint(res.second));
}

QPair<QPointF, QPointF> PageController::getPositionsForLineSelection(
    QPointF point)
{
    auto fzPoint = utils::qPointToFzPoint(point);

    auto res = m_pageGenerator.getPositionsForLineSelection(fzPoint);
    return QPointFPair(utils::fzPointToQPoint(res.first),
                       utils::fzPointToQPoint(res.second));
}

QString PageController::getTextFromSelection(const QPointF& start,
                                             const QPointF& end)
{
    auto fzStart = utils::qPointToFzPoint(start);
    auto fzEnd = utils::qPointToFzPoint(end);
    auto res = m_pageGenerator.getTextFromSelection(fzStart, fzEnd);

    return QString::fromStdString(res);
}

}  // namespace adapters::controllers