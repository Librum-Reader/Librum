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

    void setInvertColor(bool newInvertColor) override;

    QImage renderPage() override;

    bool pointIsAboveText(const QPointF& point) override;
    bool pointIsAboveLink(const QPointF& point) override;
    const char* getLinkUriAtPoint(const QPointF& point) override;

    QList<QRectF> getBufferedSelectionRects() override;
    void generateSelectionRects(QPointF start, QPointF end) override;
    void clearBufferedSelectionRects() override;

    QPair<QPointF, QPointF> getPositionsForWordSelection(QPointF start,
                                                         QPointF end) override;
    QPair<QPointF, QPointF> getPositionsForLineSelection(
        QPointF point) override;
    QString getTextFromSelection(const QPointF& start,
                                 const QPointF& end) override;

private:
    application::core::PageGenerator m_pageGenerator;

    using QPointFPair = QPair<QPointF, QPointF>;
};

}  // namespace adapters::controllers