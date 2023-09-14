#pragma once
#include <QByteArray>
#include <QColor>
#include <QJsonObject>
#include <QList>
#include <QUuid>
#include "rectf.hpp"

namespace domain::entities
{

class Highlight
{
public:
    Highlight(int pageNumber, QColor color, QUuid uuid = QUuid::createUuid());

    bool operator==(const Highlight& rhs) const;

    QUuid getUuid() const;
    int getPageNumber() const;
    QColor getColor() const;
    const QList<RectF>& getRects() const;
    void setRects(const QList<RectF>& rects);
    void setRects(const QList<QRectF>& rects);

    QByteArray toJson() const;
    static Highlight fromJson(const QJsonObject& jsonBook);

private:
    QJsonArray serializeRects() const;

    QUuid m_uuid;
    int m_pageNumber = 0;
    QColor m_color;
    QList<RectF> m_rects;
};

}  // namespace domain::entities