#pragma once
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QUuid>

namespace domain::models
{

class Tag
{
public:
    Tag(const QString& name, const QString& uuid = "");

    bool operator==(const Tag& other) const;

    const QUuid& getUuid() const;

    const QString& getName() const;
    void setName(QString newName);

public:
    QByteArray toJson();
    static Tag fromJson(const QJsonObject& jsonObject);

private:
    QUuid m_uuid;
    QString m_name;
};

}  // namespace domain::models