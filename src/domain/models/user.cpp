#include "user.hpp"


namespace domain::models
{

User::User()
{
}


const QString& User::getFirstName() const
{
    return m_firstName;
}

void User::setFirstName(const QString& newFirstName)
{
    m_firstName = newFirstName;
}

const QString& User::getLastName() const
{
    return m_lastName;
}

void User::setLastName(const QString& newLastName)
{
    m_lastName = newLastName;
}

const QString& User::getEmail() const
{
    return m_emailName;
}

void User::setEmail(const QString& newEmailName)
{
    m_emailName = newEmailName;
}

const std::vector<Tag>& User::getTags() const
{
    return m_tags;
}

void User::setTags(const std::vector<Tag>& newTags)
{
    m_tags = newTags;
}

} // namespace domain::models