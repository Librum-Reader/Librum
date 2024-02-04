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
    Folder(QString name, QString color, QString icon, QString description,
           QUuid uuid = QUuid::createUuid());
    Folder(const Folder& folder);
    Folder(Folder&& folder);

    bool operator==(const Folder& rhs) const;
    Folder& operator=(const Folder& rhs);
    Folder& operator=(Folder&& rhs);

    QUuid getUuid() const;

    QString getName() const;
    void setName(const QString& name);

    QString getColor() const;
    void setColor(const QString& color);

    QString getIcon() const;
    void setIcon(const QString& icon);

    QString getDescription() const;
    void setDescription(const QString& description);

    Folder* getParent() const;
    void setParent(Folder* parent);

    const QDateTime& getLastModified() const;
    void setLastModified(const QDateTime& newLastModified);
    void updateLastModified();

    void updateProperties(const Folder& folder);

    int getIndexInParent() const;
    int getIndexOfChild(const QUuid& uuid) const;
    bool isChildOf(const Folder& folder) const;

    const std::vector<std::unique_ptr<Folder>>& getChildren() const;
    std::vector<std::unique_ptr<Folder>>& getChildren();
    const Folder* getChild(const QUuid& uuid) const;
    Folder* getChild(const QUuid& uuid);
    void addChild(std::unique_ptr<Folder> child);
    void removeChild(const QUuid& uuid);
    const Folder* getChildAtIndex(int index) const;
    Folder* getChildAtIndex(int index);
    int childCount() const;

    QByteArray toJson() const;
    static Folder fromJson(const QJsonObject& jsonFolder, Folder* parent);

private:
    QJsonArray serializeChildren() const;

    QUuid m_uuid;
    QString m_name;
    QString m_color;
    QString m_icon;
    QString m_description;
    QDateTime m_lastModified;
    Folder* m_parent = nullptr;
    std::vector<std::unique_ptr<Folder>> m_children;

    inline static const QString dateTimeStringFormat {
        "hh:mm:ss - dd.MM.yyyy"
    };
};

}  // namespace domain::entities