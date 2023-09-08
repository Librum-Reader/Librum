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
    return m_pageGenerator.getWidth() * m_matrix.a;
}

int PageController::getHeight()
{
    return m_pageGenerator.getHeight() * m_matrix.d;
}

void PageController::setZoom(float zoom)
{
    m_matrix.a = zoom;
    m_matrix.d = zoom;

    m_pageImageOutdated = true;
    m_selectionRectsOutdated = true;
}

float PageController::getZoom()
{
    return m_matrix.a;
}

void PageController::setInvertColor(bool newInvertColor)
{
    m_pageGenerator.setInvertColor(newInvertColor);
    m_pageImageOutdated = true;
}

QImage PageController::renderPage()
{
    if(!m_pageImageOutdated)
        return m_pageImage;

    auto zoom = m_matrix.a;
    m_pageImage = utils::qImageFromPixmap(m_pageGenerator.renderPage(zoom));

    m_pageImageOutdated = false;
    return m_pageImage;
}

bool PageController::pointIsAboveText(const QPointF& point)
{
    auto fzPoint = utils::qPointToFzPoint(point);
    utils::restorePoint(fzPoint, m_matrix);

    return m_pageGenerator.pointIsAboveText(fzPoint);
}

bool PageController::pointIsAboveLink(const QPointF& point)
{
    auto fzPoint = utils::qPointToFzPoint(point);
    utils::restorePoint(fzPoint, m_matrix);

    return m_pageGenerator.pointIsAboveLink(fzPoint);
}

const char* PageController::getLinkUriAtPoint(const QPointF& point)
{
    auto fzPoint = utils::qPointToFzPoint(point);
    utils::restorePoint(fzPoint, m_matrix);

    return m_pageGenerator.getLinkAtPoint(fzPoint).uri();
}

const QList<QRectF>& PageController::getBufferedSelectionRects()
{
    if(!m_selectionRectsOutdated)
        return m_selectionRects;

    m_selectionRects.clear();
    auto& rects = m_pageGenerator.getBufferedSelectionRects();
    rects.reserve(rects.size());
    for(auto& rect : rects)
    {
        rect = rect.fz_transform_quad(m_matrix);
        m_selectionRects.append(utils::fzQuadToQRectF(rect));
    }

    m_selectionRectsOutdated = false;
    return m_selectionRects;
}

void PageController::generateSelectionRects(QPointF start, QPointF end)
{
    auto fzStart = utils::qPointToFzPoint(start);
    auto fzEnd = utils::qPointToFzPoint(end);
    utils::restorePoint(fzStart, m_matrix);
    utils::restorePoint(fzEnd, m_matrix);

    m_pageGenerator.getBufferedSelectionRects().clear();
    m_pageGenerator.generateSelectionRects(fzStart, fzEnd);

    m_selectionRectsOutdated = true;
}

void PageController::clearBufferedSelectionRects()
{
    m_pageGenerator.getBufferedSelectionRects().clear();
    m_selectionRectsOutdated = true;
}

QPair<QPointF, QPointF> PageController::getPositionsForWordSelection(
    QPointF start, QPointF end)
{
    auto fzStart = utils::qPointToFzPoint(start);
    auto fzEnd = utils::qPointToFzPoint(end);
    utils::restorePoint(fzStart, m_matrix);
    utils::restorePoint(fzEnd, m_matrix);

    auto res = m_pageGenerator.getPositionsForWordSelection(fzStart, fzEnd);
    res.first = res.first.fz_transform_point(m_matrix);
    res.second = res.second.fz_transform_point(m_matrix);

    return QPointFPair(utils::fzPointToQPoint(res.first),
                       utils::fzPointToQPoint(res.second));
}

QPair<QPointF, QPointF> PageController::getPositionsForLineSelection(
    QPointF point)
{
    auto fzPoint = utils::qPointToFzPoint(point);
    utils::restorePoint(fzPoint, m_matrix);

    auto res = m_pageGenerator.getPositionsForLineSelection(fzPoint);
    res.first = res.first.fz_transform_point(m_matrix);
    res.second = res.second.fz_transform_point(m_matrix);

    return QPointFPair(utils::fzPointToQPoint(res.first),
                       utils::fzPointToQPoint(res.second));
}

QString PageController::getTextFromSelection(const QPointF& start,
                                             const QPointF& end)
{
    auto fzStart = utils::qPointToFzPoint(start);
    auto fzEnd = utils::qPointToFzPoint(end);
    utils::restorePoint(fzStart, m_matrix);
    utils::restorePoint(fzEnd, m_matrix);

    auto res = m_pageGenerator.getTextFromSelection(fzStart, fzEnd);
    return QString::fromStdString(res);
}

}  // namespace adapters::controllers