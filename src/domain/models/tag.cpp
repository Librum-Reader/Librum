#include "tag.hpp"

namespace domain::models
{

Tag::Tag(QString name, QString uuid) :
    m_name(name)
{
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