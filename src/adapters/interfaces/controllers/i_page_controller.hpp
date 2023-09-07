#pragma once
#include <mupdf/classes.h>
#include <QImage>
#include <QObject>
#include "adapters_export.hpp"

namespace adapters
{

/**
 * The PageController is an interface for the UI to run operations on a page,
 * such as rendering, generating selections, etc.
 */
class ADAPTERS_EXPORT IPageController : public QObject
{
    Q_OBJECT

public:
    virtual ~IPageController() noexcept = default;

    virtual int getWidth() = 0;
    virtual int getHeight() = 0;

    virtual float getZoom() = 0;
    virtual void setZoom(float newZoom) = 0;

    virtual void setInvertColor(bool newInvertColor) = 0;

    virtual QImage renderPage() = 0;

    virtual bool pointIsAboveText(const QPoint& point) = 0;
    virtual bool pointIsAboveLink(const QPoint& point) = 0;

    virtual const char* getLinkUriAtPoint(const QPoint& point) = 0;

    virtual QList<QRectF>& getBufferedSelectionRects() = 0;
    virtual void generateSelectionRects(QPointF start, QPointF end) = 0;
    virtual QPair<QPointF, QPointF> getPositionsForWordSelection(
        QPointF begin, QPointF end) = 0;
    virtual QPair<QPointF, QPointF> getPositionsForLineSelection(
        QPointF point) = 0;
    virtual QString getTextFromSelection(const QPointF& start,
                                         const QPointF& end) = 0;
};

}  // namespace adapters