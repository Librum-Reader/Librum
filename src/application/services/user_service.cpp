#include "user_service.hpp"
#include <QDebug>
#include "tag.hpp"

using domain::models::Tag;
using domain::models::User;

namespace application::services
{

UserService::UserService(IUserStorageGateway* userStorageGateway) :
    m_userStorageGateway(userStorageGateway),
    m_user("x", "y", "z")
{
    connect(m_userStorageGateway, &IUserStorageGateway::finishedGettingUser,
            this, &UserService::proccessUserInformation);

    // Tag insertion
    connect(&m_user, &User::tagInsertionStarted, this,
            &UserService::tagInsertionStarted);
    connect(&m_user, &User::tagInsertionEnded, this,
            &UserService::tagInsertionEnded);

    // Tag deletion
    connect(&m_user, &User::tagDeletionStarted, this,
            &UserService::tagDeletionStarted);
    connect(&m_user, &User::tagDeletionEnded, this,
            &UserService::tagDeletionEnded);

    // Tag changed
    connect(&m_user, &User::tagsChanged, this, &UserService::tagsChanged);
}

void UserService::loadUser()
{
    m_userStorageGateway->getUser(m_authenticationToken);
}

QString UserService::getFirstName() const
{
    return m_user.getFirstName();
}

void UserService::setFirstName(const QString& newFirstName)
{
    m_user.setFirstName(newFirstName);
    m_userStorageGateway->changeFirstName(m_authenticationToken,
                                          m_user.getFirstName());
}

QString UserService::getLastName() const
{
    return m_user.getLastName();
}

void UserService::setLastName(const QString& newLastName)
{
    m_user.setLastName(newLastName);
    m_userStorageGateway->changeLastName(m_authenticationToken,
                                         m_user.getLastName());
}

QString UserService::getEmail() const
{
    return m_user.getEmail();
}

void UserService::setEmail(const QString& newEmail)
{
    m_user.setEmail(newEmail);
    m_userStorageGateway->changeEmail(m_authenticationToken, m_user.getEmail());
}

QImage UserService::getProfilePicture() const
{
    return m_user.getProfilePicture();
}

void UserService::setProfilePicture(const QImage& image)
{
    m_user.setProfilePicture(image);
    m_userStorageGateway->changeProfilePicture(m_authenticationToken, image);
}

const std::vector<domain::models::Tag>& UserService::getTags() const
{
    return m_user.getTags();
}

QUuid UserService::addTag(const domain::models::Tag& tag)
{
    m_user.addTag(tag);
    return tag.getUuid();
}

bool UserService::removeTag(const QString& tagName)
{
    auto success = m_user.removeTag(tagName);
    return success;
}

bool UserService::renameTag(const QString& oldName, const QString& newName)
{
    auto success = m_user.renameTag(oldName, newName);
    return success;
}

void UserService::proccessUserInformation(const domain::models::User& user,
                                          bool success)
{
    if(!success)
    {
        emit finishedLoadingUser(false);
        return;
    }

    m_user.setFirstName(user.getFirstName());
    m_user.setLastName(user.getLastName());
    m_user.setEmail(user.getEmail());

    for(const auto& tag : user.getTags())
    {
        m_user.addTag(tag);
    }

    emit finishedLoadingUser(true);
}

void UserService::setAuthenticationToken(const QString& token,
                                         const QString& email)
{
    Q_UNUSED(email);
    m_authenticationToken = token;
}

void UserService::clearAuthenticationToken()
{
    m_authenticationToken.clear();
}

}  // namespace application::services