#include "user_storage_gateway.hpp"
#include <QJsonDocument>
#include <QJsonObject>


using namespace domain::entities;

namespace adapters::gateways
{

UserStorageGateway::UserStorageGateway(IUserStorageAccess* userStorageAccess) :
    m_userStorageAccess(userStorageAccess)
{
    connect(m_userStorageAccess, &IUserStorageAccess::userReady, this,
            &UserStorageGateway::proccessUserData);

    connect(m_userStorageAccess, &IUserStorageAccess::gettingUserFailed, this,
            &UserStorageGateway::reportFailureGettingUser);

    connect(m_userStorageAccess, &IUserStorageAccess::profilePictureReady, this,
            &UserStorageGateway::profilePictureReady);

    connect(m_userStorageAccess, &IUserStorageAccess::passwordChangeFinished,
            this, &UserStorageGateway::passwordChangeFinished);
}

void UserStorageGateway::getUser(const QString& authToken)
{
    m_userStorageAccess->getUser(authToken);
}

void UserStorageGateway::deleteUser(const QString& authToken)
{
    m_userStorageAccess->deleteUser(authToken);
}

void UserStorageGateway::getProfilePicture(const QString& authToken)
{
    m_userStorageAccess->getProfilePicture(authToken);
}

void UserStorageGateway::changeFirstName(const QString& authToken,
                                         const QString& newFirstName)
{
    m_userStorageAccess->changeFirstName(authToken, newFirstName);
}

void UserStorageGateway::changeLastName(const QString& authToken,
                                        const QString& newLastName)
{
    m_userStorageAccess->changeLastName(authToken, newLastName);
}

void UserStorageGateway::changeEmail(const QString& authToken,
                                     const QString& newEmail)
{
    m_userStorageAccess->changeEmail(authToken, newEmail);
}

void UserStorageGateway::changePassword(const QString& authToken,
                                        const QString& newPassword)
{
    m_userStorageAccess->changePassword(authToken, newPassword);
}

void UserStorageGateway::changeProfilePicture(const QString& authToken,
                                              const QString& path)
{
    m_userStorageAccess->changeProfilePicture(authToken, path);
}

void UserStorageGateway::deleteProfilePicture(const QString& authToken)
{
    m_userStorageAccess->deleteProfilePicture(authToken);
}

void UserStorageGateway::changeProfilePictureLastUpdated(
    const QString& authToken, const QDateTime& newDateTime)
{
    m_userStorageAccess->changeProfilePictureLastUpdated(
        authToken, newDateTime.toString(m_dateTimeFormat));
}

void UserStorageGateway::changeHasProfilePicture(const QString& authToken,
                                                 bool newValue)
{
    QString newValueStr = newValue ? "true" : "false";
    m_userStorageAccess->changeHasProfilePicture(authToken, newValueStr);
}

void UserStorageGateway::deleteTag(const QString& authToken, const QUuid& uuid)
{
    m_userStorageAccess->deleteTag(authToken,
                                   uuid.toString(QUuid::WithoutBraces));
}

void UserStorageGateway::renameTag(const QString& authToken, const QUuid& uuid,
                                   const QString& newName)
{
    QJsonObject jsonTag {
        { "guid", uuid.toString(QUuid::WithoutBraces) },
        { "name", newName },
    };

    m_userStorageAccess->renameTag(authToken, jsonTag);
}

void UserStorageGateway::proccessUserData(const QByteArray& data)
{
    User user("x", "y", "z", 0, 0);

    assignValuesToUser(user, data);

    emit finishedGettingUser(user, true);
}

void UserStorageGateway::reportFailureGettingUser()
{
    User emptyUser("invalid", "invalid", "invalid@email.x", 0, 0);
    emit finishedGettingUser(emptyUser, false);
}

void UserStorageGateway::assignValuesToUser(User& user,
                                            const QByteArray& values)
{
    auto jsonDoc = QJsonDocument::fromJson(values);
    auto jsonObj = jsonDoc.object();

    user.setFirstName(jsonObj["firstName"].toString());
    user.setLastName(jsonObj["lastName"].toString());
    user.setUsedBookStorage(
        static_cast<long>(jsonObj["usedBookStorage"].toDouble()));
    user.setBookStorageLimit(
        static_cast<long>(jsonObj["bookStorageLimit"].toDouble()));
    user.setProfilePictureLastUpdated(QDateTime::fromString(
        jsonObj["profilePictureLastUpdated"].toString(), m_dateTimeFormat));
    user.setHasProfilePicture(jsonObj["hasProfilePicture"].toBool());
    user.setEmail(jsonObj["email"].toString());

    addTagsToUser(user, jsonObj["tags"].toArray());
}

void UserStorageGateway::addTagsToUser(User& user, const QJsonArray& tags)
{
    for(const auto& tag : tags)
    {
        auto jsonTagObject = tag.toObject();
        renameJsonObjectKey(jsonTagObject, "guid", "uuid");

        auto tagToAdd = Tag::fromJson(jsonTagObject);
        user.addTag(tagToAdd);
    }
}

void UserStorageGateway::renameJsonObjectKey(QJsonObject& jsonObject,
                                             const QString& oldKeyName,
                                             const QString& newKeyName)
{
    auto temp = jsonObject[oldKeyName].toString();
    jsonObject.remove(oldKeyName);
    jsonObject.insert(newKeyName, temp);
}

}  // namespace adapters::gateways
