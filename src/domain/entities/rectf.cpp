#include "rectf.hpp"
#include <QJsonDocument>

namespace domain::entities
{

RectF::RectF(QRectF rect, QUuid uuid) :
    m_uuid(uuid),
    m_rect(rect)
{
}

bool RectF::operator==(const RectF& rhs) const
{
    return m_uuid == rhs.m_uuid && m_rect == rhs.m_rect;
}

QUuid RectF::getUuid() const
{
    return m_uuid;
}

QRectF RectF::getQRect() const
{
    return m_rect;
}

void RectF::setQRect(const QRectF rect)
{
    m_rect = rect;
}

QByteArray RectF::toJson() const
{
    QJsonObject jsonRect {
        { "uuid", m_uuid.toString(QUuid::WithoutBraces) },
        { "x", m_rect.x() },
        { "y", m_rect.y() },
        { "width", m_rect.width() },
        { "height", m_rect.height() },
    };

    QJsonDocument jsonDoc(jsonRect);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);

    return jsonString.toUtf8();
}

RectF RectF::fromJson(const QJsonObject& jsonBook)
{
    auto uuid = QUuid(jsonBook["uuid"].toString());
    auto x = jsonBook["x"].toDouble();
    auto y = jsonBook["y"].toDouble();
    auto width = jsonBook["width"].toDouble();
    auto height = jsonBook["height"].toDouble();
    QRectF rect(x, y, width, height);
    RectF rectF(rect, uuid);

    return rectF;
}

}  // namespace domain::entities