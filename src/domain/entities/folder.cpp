#include "folder.hpp"
#include <QJsonDocument>

namespace domain::entities
{

Folder::Folder(QString name, int indexInParent, QUuid uuid) :
    m_uuid(uuid),
    m_name(name),
    m_indexInParent(indexInParent)
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

int Folder::getIndexInParent() const
{
    return m_indexInParent;
}

void Folder::setIndexInParent(int newIndexInParent)
{
    m_indexInParent = newIndexInParent;
}

const std::vector<std::unique_ptr<Folder> >& Folder::getChildren() const
{
    return m_children;
}

void Folder::addChild(std::unique_ptr<Folder> child)
{
    child->setParent(this);
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

Folder* Folder::getChildAtIndex(int index)
{
    return m_children[index].get();
}

int Folder::childCount() const
{
    return m_children.size();
}

QByteArray Folder::toJson() const
{
    QJsonObject folder {
        { "uuid", m_uuid.toString(QUuid::WithoutBraces) },
        { "name", m_name },
        { "indexInParent", m_indexInParent },
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
    auto indexInParent = jsonFolder["indexInParent"].toInt();
    Folder folder(name, indexInParent, uuid);
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