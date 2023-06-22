#pragma once
#include <QDate>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <optional>
#include <user.hpp>
#include "login_model.hpp"
#include "tag.hpp"

namespace application::utility
{

struct UserData
{
    QString firstName;
    QString lastName;
    QString email;
    long usedBookStorage;
    long bookStorageLimit;
    QDateTime profilePictureLastUpdated;
    std::vector<domain::entities::Tag> tags;
};

struct AuthenticationData
{
    QString email;
    QString token;
};

/**
 * The automatic login helper provides the functionality to automatically log
 * the user in. When a user for example clicks the "Remember Me" checkbox and
 * then logs in, their data is written to a file which can then be used to
 * automatically log in the next time the user starts up the application.
 *
 * The AutomaticLoginHelper checks the underlying "automatic login file" and if
 * there is data in it, it reads it in and sends it to the caller, if there is
 * no data in the "automatic login file" nothing is returned.
 */
namespace AutomaticLoginHelper
{

using domain::entities::Tag;
using domain::entities::User;
using domain::value_objects::LoginModel;

// Private namespace
namespace
{

// Members
const QString m_automaticLoginFile { "autologin.txt" };

// Private functions
std::optional<QJsonObject> getAutomaticLoginFileData()
{
    auto currentFolder = QDir::current();
    auto automaticLoginFilePath = currentFolder.filePath(m_automaticLoginFile);

    QFile file(automaticLoginFilePath);
    if(!file.open(QFile::ReadOnly | QIODevice::Text))
        return std::nullopt;

    QByteArray jsonData = file.readAll();

    QJsonParseError parseError;
    auto jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
    if(parseError.error != QJsonParseError::NoError)
    {
        qWarning() << QString("Error parsing automatic login file");
        return std::nullopt;
    }

    return jsonDoc.object();
}

void appendUserDataToJsonObject(QJsonObject& jsonObject, UserData userData)
{
    jsonObject.insert("firstName", userData.firstName);
    jsonObject.insert("lastName", userData.lastName);
    jsonObject.insert("email", userData.email);
    jsonObject.insert("usedBookStorage",
                      static_cast<qint64>(userData.usedBookStorage));
    jsonObject.insert("bookStorageLimit",
                      static_cast<qint64>(userData.bookStorageLimit));
    jsonObject.insert("profilePictureLastUpdated",
                      userData.profilePictureLastUpdated.toString());

    QJsonArray tags;
    for(const auto& tag : userData.tags)
    {
        auto obj = QJsonDocument::fromJson(tag.toJson()).object();
        tags.append(QJsonValue::fromVariant(obj));
    }
    jsonObject.insert("tags", tags);
}

void saveDataToAutomaticLoginFile(const QJsonObject& data)
{
    auto currentFolder = QDir::current();
    auto automaticLoginFilePath = currentFolder.filePath(m_automaticLoginFile);

    QFile file(automaticLoginFilePath);
    if(!file.open(QFile::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed Opening automatic login file for saving data";
        return;
    }

    QJsonDocument jsonDoc(data);
    file.write(jsonDoc.toJson((QJsonDocument::Indented)));
}


}  // namespace

inline std::optional<AuthenticationData> tryAutomaticAuthentication()
{
    auto jsonData = getAutomaticLoginFileData();
    if(!jsonData.has_value() || jsonData.value().isEmpty())
        return std::nullopt;

    QJsonObject automaticLoginData = jsonData.value();
    AuthenticationData authData {
        automaticLoginData["email"].toString(),
        automaticLoginData["token"].toString(),
    };

    return authData;
}

// Public functions
inline std::optional<UserData> tryAutomaticUserLoading()
{
    auto jsonData = getAutomaticLoginFileData();
    // Check if the data contains the "firstName" property since it should be
    // set if the user was already saved. We can't check for no-saved-data,
    // since the authentication data is saved before the user data, thus there
    // is already some data in the file, even though the user was not saved.
    if(!jsonData.has_value() || !jsonData.value().contains("firstName"))
        return std::nullopt;

    QJsonObject automaticLoginData = jsonData.value();
    UserData user {
        automaticLoginData["firstName"].toString(),
        automaticLoginData["lastName"].toString(),
        automaticLoginData["email"].toString(),
        static_cast<long>(automaticLoginData["usedBookStorage"].toDouble()),
        static_cast<long>(automaticLoginData["bookStorageLimit"].toDouble()),
        QDateTime::fromString(
            automaticLoginData["profilePictureLastUpdated"].toString()),
        std::vector<Tag>(),
    };

    // Add tags
    for(const auto& jsonTag : automaticLoginData["tags"].toArray())
    {
        auto tagObject = jsonTag.toObject();
        Tag tag = Tag::fromJson(tagObject);

        user.tags.push_back(tag);
    }

    return user;
}

inline void clearAutomaticLoginData()
{
    auto currentFolder = QDir::current();
    auto automaticLoginFilePath = currentFolder.filePath(m_automaticLoginFile);

    QFile file(automaticLoginFilePath);
    file.resize(0);  // Delete all contents of the file
}

inline void saveAuthenticationData(const AuthenticationData& authData)
{
    // Make sure to clear the previous authentication data before saving the new
    clearAutomaticLoginData();


    QJsonObject jsonAuthData {
        { "email", authData.email },
        { "token", authData.token },
    };
    saveDataToAutomaticLoginFile(jsonAuthData);
}

inline void saveUserData(const UserData& userData)
{
    // User data can only be appended to the file, if the authentication data
    // already exists. So check if the file already contains the auth data.
    auto existingAuthData = getAutomaticLoginFileData();
    if(!existingAuthData.has_value())
        return;

    auto jsonUserData = existingAuthData.value();
    appendUserDataToJsonObject(jsonUserData, userData);

    saveDataToAutomaticLoginFile(jsonUserData);
}

}  // namespace AutomaticLoginHelper

}  // namespace application::utility