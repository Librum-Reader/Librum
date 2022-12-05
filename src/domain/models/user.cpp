#include "user.hpp"
#include <ranges>

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

const Tag* User::getTag(const QString& tagName) const
{
    auto tagPosition = std::ranges::find(m_tags, Tag(tagName));
    if(tagPosition == m_tags.end())
        return nullptr;

    return &(*tagPosition);
}

bool User::addTag(const Tag& tag)
{
    auto tagPosition = std::ranges::find(m_tags, tag);
    if(tagPosition != m_tags.end())
        return false;

    emit tagInsertionStarted(m_tags.size());
    m_tags.emplace_back(tag);
    emit tagInsertionEnded();

    return true;
}

bool User::removeTag(const QString& tagName)
{
    auto tagPosition = std::ranges::find(m_tags, Tag(tagName));
    if(tagPosition == m_tags.end())
        return false;

    auto index = getTagIndex(tagName);

    emit tagDeletionStarted(index);
    m_tags.erase(tagPosition);
    emit tagDeletionEnded();

    return true;
}

bool User::renameTag(const QString& oldName, const QString& newName)
{
    auto tagPosition = std::ranges::find(m_tags, Tag(oldName));
    if(tagPosition == m_tags.end())
        return false;

    auto index = getTagIndex(oldName);
    m_tags[index].setName(newName);
    emit tagsChanged(index);

    return true;
}

int User::getTagIndex(const QString& tagName)
{
    auto* tag = getTag(tagName);
    if(!tag)
        return -1;

    std::vector<Tag>::const_iterator tagPosition(tag);
    size_t index = tagPosition - getTags().begin();

    return index;
}

}  // namespace domain::models