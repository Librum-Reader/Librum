#include "highlight.hpp"
#include <QJsonArray>
#include <QJsonDocument>

namespace domain::entities
{

Highlight::Highlight(int pageNumber, QColor color, QUuid uuid) :
    m_uuid(uuid),
    m_pageNumber(pageNumber),
    m_color(color)
{
}

QByteArray Highlight::toJson() const
{
    QJsonObject book {
        { "uuid", m_uuid.toString(QUuid::WithoutBraces) },
        { "pageNumber", m_pageNumber },
        { "color", m_color.name(QColor::HexArgb) },
        { "rects", serializeRects() },
    };

    QJsonDocument doc(book);
    QString strJson = doc.toJson(QJsonDocument::Indented);

    return strJson.toUtf8();
}

Highlight Highlight::fromJson(const QJsonObject& jsonBook)
{
    auto uuid = QUuid(jsonBook["uuid"].toString());
    auto pageNumber = jsonBook["pageNumber"].toInt();
    auto color = QColor(jsonBook["color"].toString());
    Highlight highlight(pageNumber, color, uuid);

    auto jsonRects = jsonBook["rects"].toArray();
    for(const auto& jsonRect : jsonRects)
    {
        auto rect = qRectFFromJson(jsonRect.toObject());
        highlight.m_rects.append(rect);
    }

    return highlight;
}

QUuid Highlight::getUuid() const
{
    return m_uuid;
}

int Highlight::getPageNumber() const
{
    return m_pageNumber;
}

QColor Highlight::getColor() const
{
    return m_color;
}

const QList<QRectF>& Highlight::getRects() const
{
    return m_rects;
}

void Highlight::setRects(const QList<QRectF>& rects)
{
    m_rects = rects;
}

QJsonArray Highlight::serializeRects() const
{
    QJsonArray result;
    for(const auto& rect : m_rects)
    {
        auto obj = QJsonDocument::fromJson(qRectFToJson(rect)).object();
        result.append(QJsonValue::fromVariant(obj));
    }

    return result;
}

QByteArray Highlight::qRectFToJson(const QRectF& rect) const
{
    QJsonObject jsonRect {
        { "x", rect.x() },
        { "y", rect.y() },
        { "width", rect.width() },
        { "height", rect.height() },
    };

    QJsonDocument jsonDoc(jsonRect);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);

    return jsonString.toUtf8();
}

QRectF Highlight::qRectFFromJson(const QJsonObject& jsonRect)
{
    float x = jsonRect["x"].toDouble();
    float y = jsonRect["y"].toDouble();
    float width = jsonRect["width"].toDouble();
    float height = jsonRect["height"].toDouble();
    QRectF rect(x, y, width, height);

    return rect;
}

}  // namespace domain::entities