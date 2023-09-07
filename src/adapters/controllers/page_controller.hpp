#pragma once
#include <QObject>
#include "i_page_controller.hpp"
#include "page_generator.hpp"

namespace adapters::controllers
{

class PageController : public IPageController
{
    Q_OBJECT

public:
    PageController(mupdf::FzDocument* document, int pageNumber);

    int getWidth() override;
    int getHeight() override;

    float getZoom() override;
    void setZoom(float newZoom) override;

    QImage renderPage() override;
    QPointF scalePointToCurrentZoom(const QPointF& point,
                                    float oldZoom) override;
    bool pointIsAboveText(const QPoint& point) override;
    bool pointIsAboveLink(const QPoint& point) override;
    const char* getLinkUriAtPoint(const QPoint& point) override;
    QList<QRectF>& getBufferedSelectionRects() override;
    void setInvertColor(bool newInvertColor) override;
    void generateSelectionRects(QPointF start, QPointF end) override;
    QPair<QPointF, QPointF> getPositionsForWordSelection(QPointF begin,
                                                         QPointF end) override;
    QPair<QPointF, QPointF> getPositionsForLineSelection(
        QPointF point) override;
    QString getTextFromSelection(const QPointF& start,
                                 const QPointF& end) override;

private:
    application::core::PageGenerator m_pageGenerator;
};

}  // namespace adapters::controllers