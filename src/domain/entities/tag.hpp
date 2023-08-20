#pragma once
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QUuid>
#include "domain_export.hpp"

namespace domain::entities
{

class DOMAIN_EXPORT Tag
{
public:
     explicit Tag(const QString& name, const QString& uuid = "");

     bool operator==(const Tag& other) const;

     const QUuid& getUuid() const;
     const QString& getName() const;
     void setName(QString newName);

     bool isValid() const;
     QByteArray toJson() const;
     static Tag fromJson(const QJsonObject& jsonObject);

private:
    // Tag names should always start with a capital letter
    void capitalizeName(QString& tagName);

    QUuid m_uuid;
    QString m_name;
};

}  // namespace domain::entities
