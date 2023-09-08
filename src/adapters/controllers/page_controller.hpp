#pragma once
#include <QObject>
#include "i_page_controller.hpp"
#include "mupdf/classes.h"
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

    void setZoom(float zoom) override;
    float getZoom() override;

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
    mupdf::FzMatrix m_matrix;

    // Image caching
    bool m_pageImageOutdated = true;
    QImage m_pageImage;

    // Selection rects outdated
    bool m_selectionRectsOutdated = true;
    QList<QRectF> m_selectionRects;

    using QPointFPair = QPair<QPointF, QPointF>;
};

}  // namespace adapters::controllers