#pragma once
#include <QJsonObject>
#include <QString>
#include <QUuid>
#include "domain_export.hpp"

namespace domain::entities
{

class DOMAIN_EXPORT Bookmark
{
public:
    Bookmark(const QString& name, int pageNumber, float yOffset,
             const QString& uuid = "");

    bool operator==(const Bookmark& rhs) const;

    QUuid getUuid() const;
    QString getName() const;
    void setName(const QString& newName);
    int getPageNumber() const;
    void setPageNumber(int newPageNumber);
    float getYOffset() const;
    void setYOffset(float newYOffset);

    QByteArray toJson() const;
    static Bookmark fromJson(const QJsonObject& jsonBookmark);

private:
    QUuid m_uuid;
    QString m_name;
    int m_pageNumber;
    float m_yOffset;
};

}  // namespace domain::entities