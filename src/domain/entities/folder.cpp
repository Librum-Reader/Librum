#include "folder.hpp"
#include <QJsonDocument>

namespace domain::entities
{

Folder::Folder(QString name, QUuid uuid) :
    m_uuid(uuid),
    m_name(name)
{
}

bool Folder::operator==(const Folder& rhs) const
{
    return m_uuid == rhs.m_uuid && m_name == rhs.m_name &&
           m_parent == rhs.m_parent && m_children == rhs.m_children;
}

QUuid Folder::getUuid() const
{
    return m_uuid;
}

QString Folder::getName() const
{
    return m_name;
}

void Folder::setName(const QString& name)
{
    m_name = name;
}

Folder* Folder::getParent() const
{
    return m_parent;
}

void Folder::setParent(Folder* parent)
{
    m_parent = parent;
}

void Folder::addChild(std::unique_ptr<Folder> child)
{
    m_children.emplace_back(std::move(child));
}

void Folder::removeChild(const QUuid& uuid)
{
    auto it = std::find_if(m_children.begin(), m_children.end(),
                           [uuid](const auto& child)
                           {
                               return child->getUuid() == uuid;
                           });

    if(it != m_children.end())
    {
        m_children.erase(it);
    }
}

QByteArray Folder::toJson() const
{
    QJsonObject folder {
        { "uuid", m_uuid.toString(QUuid::WithoutBraces) },
        { "name", m_name },
        { "children", serializeChildren() },
    };

    QJsonDocument doc(folder);
    QString strJson = doc.toJson(QJsonDocument::Indented);

    return strJson.toUtf8();
}

QJsonArray Folder::serializeChildren() const
{
    QJsonArray result;
    for(const auto& child : m_children)
    {
        auto obj = QJsonDocument::fromJson(child->toJson()).object();
        result.append(QJsonValue::fromVariant(obj));
    }

    return result;
}

Folder Folder::fromJson(const QJsonObject& jsonFolder, Folder* parent)
{
    auto uuid = QUuid(jsonFolder["uuid"].toString());
    auto name = jsonFolder["name"].toString();
    Folder folder(name, uuid);
    folder.setParent(parent);

    auto jsonChildren = jsonFolder["children"].toArray();
    for(const auto& jsonChild : jsonChildren)
    {
        // Recursively fill up all children
        auto rect = Folder::fromJson(jsonChild.toObject(), &folder);
    }

    return folder;
}

}  // namespace domain::entities