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

bool Highlight::operator==(const Highlight& rhs) const
{
    return m_uuid == rhs.m_uuid && m_pageNumber == rhs.m_pageNumber &&
           m_color == rhs.m_color && m_rects == rhs.m_rects;
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

QJsonArray Highlight::serializeRects() const
{
    QJsonArray result;
    for(const auto& rect : m_rects)
    {
        auto obj = QJsonDocument::fromJson(rect.toJson()).object();
        result.append(QJsonValue::fromVariant(obj));
    }

    return result;
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
        auto rect = RectF::fromJson(jsonRect.toObject());
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

const QList<RectF>& Highlight::getRects() const
{
    return m_rects;
}

void Highlight::setRects(const QList<RectF>& rects)
{
    m_rects = rects;
}

void Highlight::setRects(const QList<QRectF>& rects)
{
    m_rects.clear();
    m_rects.reserve(rects.count());
    for(const auto& rect : rects)
    {
        m_rects.append(RectF(rect));
    }
}

}  // namespace domain::entities