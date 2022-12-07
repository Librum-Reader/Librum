#include "tag.hpp"

namespace domain::models
{

Tag::Tag(const QString& name, const QString& uuid) :
    m_name(name)
{
    // Capitalize by default m_name
    m_name.at(0).toUpper();

    // Generate uuid if it's not provided, else assign
    if(uuid.isEmpty())
        m_uuid = QUuid::createUuid();
    else
        m_uuid = QUuid(uuid);
}

bool Tag::operator==(const Tag& other) const
{
    return m_name == other.m_name;
}

const QUuid& Tag::getUuid() const
{
    return m_uuid;
}

const QString& Tag::getName() const
{
    return m_name;
}

void Tag::setName(QString newName)
{
    m_name = newName;
}

}  // namespace domain::models