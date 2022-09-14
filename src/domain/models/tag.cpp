#include "tag.hpp"


namespace domain::models
{

Tag::Tag(QString name)
    : m_name(name)
{
}


bool Tag::operator==(const Tag& other)
{
    return m_name == other.m_name;
}


QString Tag::name()
{
    return m_name;
}

void Tag::setName(QString newName)
{
    m_name = newName;
}   

} // namespace domain::models