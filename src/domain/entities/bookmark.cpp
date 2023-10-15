#include "bookmark.hpp"
#include <QJsonDocument>

namespace domain::entities
{

Bookmark::Bookmark(const QString& name, int pageNumber, float yOffset,
                   const QString& uuid) :
    m_name(name),
    m_pageNumber(pageNumber),
    m_yOffset(yOffset)
{
    if(uuid.isEmpty())
        m_uuid = QUuid::createUuid();
    else
        m_uuid = QUuid(uuid);
}

bool Bookmark::operator==(const Bookmark& rhs) const
{
    return m_uuid == rhs.m_uuid && m_name == rhs.m_name &&
           m_pageNumber == rhs.m_pageNumber && m_yOffset == rhs.m_yOffset;
}

QUuid Bookmark::getUuid() const
{
    return m_uuid;
}

QString Bookmark::getName() const
{
    return m_name;
}

void Bookmark::setName(const QString& newName)
{
    m_name = newName;
}

int Bookmark::getPageNumber() const
{
    return m_pageNumber;
}

void Bookmark::setPageNumber(int newPageNumber)
{
    m_pageNumber = newPageNumber;
}

float Bookmark::getYOffset() const
{
    return m_yOffset;
}

void Bookmark::setYOffset(float newYOffset)
{
    m_yOffset = newYOffset;
}

QByteArray Bookmark::toJson() const
{
    QJsonObject bookmark {
        { "uuid", m_uuid.toString(QUuid::WithoutBraces) },
        { "name", m_name },
        { "pageNumber", m_pageNumber },
        { "yOffset", m_yOffset },
    };

    QJsonDocument doc(bookmark);
    QString strJson = doc.toJson(QJsonDocument::Indented);

    return strJson.toUtf8();
}

Bookmark Bookmark::fromJson(const QJsonObject& jsonBookmark)
{
    auto uuid = jsonBookmark["uuid"].toString();
    auto name = jsonBookmark["name"].toString();
    auto pageNumber = jsonBookmark["pageNumber"].toInt();
    auto yOffset = jsonBookmark["yOffset"].toDouble();
    Bookmark bookmark(name, pageNumber, yOffset, uuid);

    return bookmark;
}

}  // namespace domain::entities