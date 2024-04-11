#include "page_controller.hpp"
#include <QDebug>
#include "fz_utils.hpp"
#include "mupdf/classes.h"

using namespace application::core;

namespace adapters::controllers
{

PageController::PageController(mupdf::FzDocument* document, int pageNumber,
                               double dpr) :
    m_pageGenerator(document, pageNumber),
    m_dpr(dpr)
{
}

int PageController::getWidth()
{
    return m_pageGenerator.getWidth() * m_matrix.a / m_dpr;
}

int PageController::getHeight()
{
    return m_pageGenerator.getHeight() * m_matrix.d / m_dpr;
}

int PageController::getXOffset() const
{
    return m_pageXOffset;
}

int PageController::getYOffset() const
{
    return m_pageYOffset;
}

void PageController::setZoom(float zoom)
{
    m_matrix.a = zoom * m_dpr;
    m_matrix.d = zoom * m_dpr;

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

QImage PageController::renderPage(int pageNr)
{
    if(!m_pageImageOutdated)
        return m_pageImage;

    qDebug() << "Re-rendering page nr: " << pageNr;

    auto zoom = m_matrix.a;
    m_pageImage = utils::qImageFromPixmap(m_pageGenerator.renderPage(zoom));

    auto xOffset = m_pageGenerator.getPageXOffset();
    auto yOffset = m_pageGenerator.getPageYOffset();
    if(xOffset != m_pageXOffset || yOffset != m_pageYOffset)
    {
        m_pageXOffset = xOffset;
        m_pageYOffset = yOffset;
        emit pageOffsetsChanged(xOffset, yOffset);
    }

    m_pageImageOutdated = false;
    return m_pageImage;
}

bool PageController::pointIsAboveText(const QPointF& point)
{
    auto fzPoint = utils::qPointToFzPoint(point, m_dpr);
    utils::restoreFzPoint(fzPoint, m_matrix);

    return m_pageGenerator.pointIsAboveText(fzPoint);
}

bool PageController::pointIsAboveLink(const QPointF& point)
{
    auto fzPoint = utils::qPointToFzPoint(point, m_dpr);
    utils::restoreFzPoint(fzPoint, m_matrix);

    return m_pageGenerator.pointIsAboveLink(fzPoint);
}

const char* PageController::getLinkUriAtPoint(const QPointF& point)
{
    auto fzPoint = utils::qPointToFzPoint(point, m_dpr);
    utils::restoreFzPoint(fzPoint, m_matrix);

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
    auto fzStart = utils::qPointToFzPoint(start, m_dpr);
    auto fzEnd = utils::qPointToFzPoint(end, m_dpr);
    utils::restoreFzPoint(fzStart, m_matrix);
    utils::restoreFzPoint(fzEnd, m_matrix);

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
    auto fzStart = utils::qPointToFzPoint(start, m_dpr);
    auto fzEnd = utils::qPointToFzPoint(end, m_dpr);
    utils::restoreFzPoint(fzStart, m_matrix);
    utils::restoreFzPoint(fzEnd, m_matrix);

    auto res = m_pageGenerator.getPositionsForWordSelection(fzStart, fzEnd);
    res.first = res.first.fz_transform_point(m_matrix);
    res.second = res.second.fz_transform_point(m_matrix);

    return QPointFPair(utils::fzPointToQPoint(res.first, m_dpr),
                       utils::fzPointToQPoint(res.second, m_dpr));
}

QPair<QPointF, QPointF> PageController::getPositionsForLineSelection(
    QPointF point)
{
    auto fzPoint = utils::qPointToFzPoint(point, m_dpr);
    utils::restoreFzPoint(fzPoint, m_matrix);

    auto res = m_pageGenerator.getPositionsForLineSelection(fzPoint);
    res.first = res.first.fz_transform_point(m_matrix);
    res.second = res.second.fz_transform_point(m_matrix);

    return QPointFPair(utils::fzPointToQPoint(res.first, m_dpr),
                       utils::fzPointToQPoint(res.second, m_dpr));
}

QString PageController::getTextFromSelection(const QPointF& start,
                                             const QPointF& end)
{
    auto fzStart = utils::qPointToFzPoint(start, m_dpr);
    auto fzEnd = utils::qPointToFzPoint(end, m_dpr);
    utils::restoreFzPoint(fzStart, m_matrix);
    utils::restoreFzPoint(fzEnd, m_matrix);

    auto res = m_pageGenerator.getTextFromSelection(fzStart, fzEnd);
    return QString::fromStdString(res);
}

}  // namespace adapters::controllers
