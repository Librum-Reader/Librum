#pragma once
#include <QJsonObject>
#include <QRectF>
#include <QUuid>

namespace domain::entities
{

class RectF
{
public:
    RectF(QRectF rect, QUuid uuid = QUuid::createUuid());

    bool operator==(const RectF& rhs) const;

    QUuid getUuid() const;
    QRectF getQRect() const;
    void setQRect(const QRectF rect);

    QByteArray toJson() const;
    static RectF fromJson(const QJsonObject& jsonBook);

private:
    QUuid m_uuid;
    QRectF m_rect;
};

}  // namespace domain::entities