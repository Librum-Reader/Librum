#pragma once
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

private:
    QUuid m_uuid;
    QString m_name;
};

}  // namespace domain::models