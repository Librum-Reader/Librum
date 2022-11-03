#include "user.hpp"

namespace domain::models
{

User::User()
{
}

User::User(const QString& firstName, const QString& lastName,
           const QString& email) :
    m_firstName(firstName),
    m_lastName(lastName),
    m_email(email)
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

bool User::addTag(const Tag& tag)
{
    auto tagPosition = std::find(m_tags.begin(), m_tags.end(), tag);
    if(tagPosition != m_tags.end())
        return false;

    m_tags.emplace_back(tag);
    return true;
}

bool User::removeTag(const Tag& tag)
{
    auto tagPosition = std::find(m_tags.begin(), m_tags.end(), tag);
    if(tagPosition == m_tags.end())
        return false;

    m_tags.erase(tagPosition);
    return true;
}

bool User::renameTag(const QString& newName)
{
    auto tagPosition = std::find_if(m_tags.begin(), m_tags.end(),
                                    [&name = newName](const Tag& tag)
                                    {
                                        return tag.getName() == name;
                                    });
    if(tagPosition == m_tags.end())
        return false;

    auto index = tagPosition - m_tags.begin();
    m_tags[index].setName(newName);

    return true;
}

}  // namespace domain::models