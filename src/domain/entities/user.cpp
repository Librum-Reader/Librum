#include "user.hpp"
#include <ranges>

namespace domain::entities
{

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

const Tag* User::getTagByUuid(const QUuid& uuid) const
{
    auto tagPosition = std::ranges::find_if(m_tags,
                                            [&uuid](const Tag& tag)
                                            {
                                                return tag.getUuid() == uuid;
                                            });
    if(tagPosition == m_tags.end())
        return nullptr;

    return &(*tagPosition);
}

const Tag* User::getTagByName(const QString& tagName) const
{
    auto tagPosition = std::ranges::find_if(m_tags,
                                            [&tagName](const Tag& tag)
                                            {
                                                return tag.getName() == tagName;
                                            });
    if(tagPosition == m_tags.end())
        return nullptr;

    return &(*tagPosition);
}

bool User::addTag(const Tag& tag)
{
    if(auto t = getTagByName(tag.getName()); t != nullptr)
        return false;

    if(!tag.isValid())
        return false;

    emit tagInsertionStarted(m_tags.size());
    m_tags.emplace_back(tag);
    emit tagInsertionEnded();

    return true;
}

bool User::deleteTag(const QUuid& uuid)
{
    auto tagPosition = std::ranges::find_if(m_tags,
                                            [uuid](const Tag& tag)
                                            {
                                                return tag.getUuid() == uuid;
                                            });
    if(tagPosition == m_tags.end())
        return false;

    emit tagDeletionStarted(getTagIndex(uuid));
    m_tags.erase(tagPosition);
    emit tagDeletionEnded();

    return true;
}

bool User::renameTag(const QUuid& uuid, const QString& newName)
{
    if(auto tag = getTagByUuid(uuid); tag == nullptr)
        return false;

    if(auto tag = getTagByName(newName); tag != nullptr)
        return false;

    auto index = getTagIndex(uuid);
    m_tags[index].setName(newName);
    emit tagsChanged(index);

    return true;
}

int User::getTagIndex(const QUuid& uuid)
{
    auto tag = getTagByUuid(uuid);
    if(!tag)
        return -1;

    std::vector<Tag>::const_iterator tagPosition(tag);
    size_t index = tagPosition - getTags().begin();

    return index;
}

}  // namespace domain::entities