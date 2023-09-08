#pragma once
#include <QByteArray>
#include <QColor>
#include <QJsonObject>
#include <QList>
#include <QRectF>
#include <QUuid>

namespace domain::entities
{

class Highlight
{
public:
    Highlight(int pageNumber, QColor color, QUuid uuid = QUuid::createUuid());

    QUuid getUuid() const;
    int getPageNumber() const;
    QColor getColor() const;
    const QList<QRectF>& getRects() const;
    void setRects(const QList<QRectF>& rects);

    QByteArray toJson() const;
    static Highlight fromJson(const QJsonObject& jsonBook);

private:
    QUuid m_uuid;
    int m_pageNumber = 0;
    QColor m_color;
    QList<QRectF> m_rects;

    QJsonArray serializeRects() const;
    QByteArray qRectFToJson(const QRectF& rect) const;
    static QRectF qRectFFromJson(const QJsonObject& jsonRect);
};

}  // namespace domain::entities