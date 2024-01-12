#include "folder.hpp"
#include <QJsonDocument>

namespace domain::entities
{

Folder::Folder(QString name, QString color, QString icon, QString description,
               QUuid uuid) :
    m_uuid(uuid),
    m_name(name),
    m_color(color),
    m_icon(icon),
    m_description(description)
{
}

Folder::Folder(const Folder& folder) :
    m_uuid(folder.m_uuid),
    m_name(folder.m_name),
    m_color(folder.m_color),
    m_icon(folder.m_icon),
    m_description(folder.m_description),
    m_parent(nullptr)
{
    for(const auto& child : folder.m_children)
    {
        auto newChild = std::make_unique<Folder>(*child);
        newChild->setParent(this);
        m_children.emplace_back(std::move(newChild));
    }
}

Folder::Folder(Folder&& folder) :
    m_uuid(std::move(folder.m_uuid)),
    m_name(std::move(folder.m_name)),
    m_color(std::move(folder.m_color)),
    m_icon(std::move(folder.m_icon)),
    m_description(std::move(folder.m_description)),
    m_parent(nullptr)
{
    for(const auto& child : folder.m_children)
    {
        auto newChild = std::make_unique<Folder>(std::move(*child));
        newChild->setParent(this);
        m_children.emplace_back(std::move(newChild));
    }
}

Folder& Folder::operator=(const Folder& rhs)
{
    if(this == &rhs)
        return *this;

    m_uuid = rhs.m_uuid;
    m_name = rhs.m_name;
    m_color = rhs.m_color;
    m_icon = rhs.m_icon;
    m_description = rhs.m_description;
    m_parent = nullptr;

    for(const auto& child : rhs.m_children)
    {
        auto newChild = std::make_unique<Folder>(*child);
        newChild->setParent(this);
        m_children.emplace_back(std::move(newChild));
    }

    return *this;
}

Folder& Folder::operator=(Folder&& rhs)
{
    if(this == &rhs)
        return *this;

    m_uuid = std::move(rhs.m_uuid);
    m_name = std::move(rhs.m_name);
    m_color = std::move(rhs.m_color);
    m_icon = std::move(rhs.m_icon);
    m_description = std::move(rhs.m_description);
    m_parent = nullptr;

    for(const auto& child : rhs.m_children)
    {
        auto newChild = std::make_unique<Folder>(std::move(*child));
        newChild->setParent(this);
        m_children.emplace_back(std::move(newChild));
    }

    return *this;
}

bool Folder::operator==(const Folder& rhs) const
{
    return m_uuid == rhs.m_uuid && m_name == rhs.m_name &&
           m_color == rhs.m_color && m_icon == rhs.m_icon &&
           m_description == rhs.m_description && m_parent == rhs.m_parent &&
           m_children == rhs.m_children;
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

QString Folder::getColor() const
{
    return m_color;
}

void Folder::setColor(const QString& color)
{
    m_color = color;
}

QString Folder::getIcon() const
{
    return m_icon;
}

void Folder::setIcon(const QString& icon)
{
    m_icon = icon;
}

QString Folder::getDescription() const
{
    return m_description;
}

void Folder::setDescription(const QString& description)
{
    m_description = description;
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
    return m_parent->getIndexOfChild(m_uuid);
}

int Folder::getIndexOfChild(const QUuid& uuid)
{
    for(std::size_t i = 0; i < m_children.size(); ++i)
    {
        if(m_children[i]->getUuid() == uuid)
            return i;
    }

    return -1;
}

bool Folder::isChildOf(const Folder& folder) const
{
    QUuid rootUuid = QUuid();

    auto currFolder = this;
    while(currFolder->getUuid() != rootUuid)
    {
        if(currFolder->getUuid() == folder.getUuid())
            return true;

        currFolder = currFolder->getParent();
    }

    return false;
}

const std::vector<std::unique_ptr<Folder> >& Folder::getChildren() const
{
    return m_children;
}

std::vector<std::unique_ptr<Folder> >& Folder::getChildren()
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
        { "color", m_color },
        { "icon", m_icon },
        { "description", m_description },
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
    auto color = jsonFolder["color"].toString();
    auto icon = jsonFolder["icon"].toString();
    auto description = jsonFolder["description"].toString();
    Folder folder(name, color, icon, description, uuid);
    folder.setParent(parent);

    auto jsonChildren = jsonFolder["children"].toArray();
    for(const auto& jsonChild : jsonChildren)
    {
        // Recursively fill up all children
        auto childFolder = Folder::fromJson(jsonChild.toObject(), &folder);
        folder.addChild(std::make_unique<Folder>(std::move(childFolder)));
    }

    return folder;
}

}  // namespace domain::entities