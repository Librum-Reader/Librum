#pragma once
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QUuid>
#include <memory>
#include <vector>

namespace domain::entities
{

class Folder
{
public:
    Folder(QString name, QUuid uuid = QUuid::createUuid());

    bool operator==(const Folder& rhs) const;

    QUuid getUuid() const;

    QString getName() const;
    void setName(const QString& name);

    Folder* getParent() const;
    void setParent(Folder* parent);

    int getIndexInParent() const;

    int getIndexOfChild(const QUuid& uuid);

    const std::vector<std::unique_ptr<Folder>>& getChildren() const;
    void addChild(std::unique_ptr<Folder> child);
    void removeChild(const QUuid& uuid);
    Folder* getChildAtIndex(int index);
    int childCount() const;

    QByteArray toJson() const;
    static Folder fromJson(const QJsonObject& jsonFolder, Folder* parent);

private:
    QJsonArray serializeChildren() const;

    QUuid m_uuid;
    QString m_name;
    Folder* m_parent = nullptr;
    std::vector<std::unique_ptr<Folder>> m_children;
};

}  // namespace domain::entities