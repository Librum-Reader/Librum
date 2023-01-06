#include "user_service.hpp"
#include <QDebug>
#include "tag.hpp"

using domain::entities::Tag;
using domain::entities::User;

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

    // Fetch changes timer
    m_fetchChangesTimer.setInterval(15'000);
    connect(&m_fetchChangesTimer, &QTimer::timeout, this,
            &UserService::loadUser);
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

const std::vector<domain::entities::Tag>& UserService::getTags() const
{
    return m_user.getTags();
}

QUuid UserService::addTag(const domain::entities::Tag& tag)
{
    auto success = m_user.addTag(tag);
    if(!success)
        return QUuid();

    return tag.getUuid();
}

bool UserService::deleteTag(const QUuid& uuid)
{
    auto success = m_user.deleteTag(uuid);
    if(success)
        m_userStorageGateway->deleteTag(m_authenticationToken, uuid);

    return success;
}

bool UserService::renameTag(const QUuid& uuid, const QString& newName)
{
    auto success = m_user.renameTag(uuid, newName);
    if(success)
        m_userStorageGateway->renameTag(m_authenticationToken, uuid, newName);

    return success;
}

void UserService::proccessUserInformation(const domain::entities::User& user,
                                          bool success)
{
    // Avoid storing data for logged out users by verifying login status before
    // applying data, else their data might be in memory even though logged out.
    if(!userIsLoggedIn() || !success)
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

bool UserService::userIsLoggedIn()
{
    return !m_authenticationToken.isEmpty();
}

void UserService::setupUserData(const QString& token, const QString& email)
{
    Q_UNUSED(email);
    m_authenticationToken = token;

    m_fetchChangesTimer.start();
}

void UserService::clearUserData()
{
    m_fetchChangesTimer.stop();
    m_user.clearData();
    m_authenticationToken.clear();
}

}  // namespace application::services