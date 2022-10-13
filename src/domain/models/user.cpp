#include "user.hpp"


namespace domain::models
{

User::User()
{
}

User::User(const QString& firstName, const QString& lastName,
           const QString& email)
    : m_firstName(firstName), m_lastName(lastName), m_email(email)
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
    return m_email;
}

void User::setEmail(const QString& newEmail)
{
    m_email = newEmail;
}

const QImage& User::getProfilePicture() const
{
    return m_profilePicture;
}

void User::setProfilePicture(const QImage& newProfilePicture)
{
    m_profilePicture = newProfilePicture;
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