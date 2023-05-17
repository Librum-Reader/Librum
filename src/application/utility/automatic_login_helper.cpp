#include "automatic_login_helper.hpp"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using domain::entities::Tag;
using domain::entities::User;
using domain::value_objects::LoginModel;

namespace application::utility
{

std::optional<AuthenticationData>
    AutomaticLoginHelper::tryAutomaticAuthentication()
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

std::optional<UserData> AutomaticLoginHelper::tryAutomaticUserLoading()
{
    auto jsonData = getAutomaticLoginFileData();
    // Check if the data contains the "firstName" property since it should be
    // set if the user was already saved. We can't check for "no saved data",
    // since the authentication data is saved before the user data, thus there
    // is already some data in the file, even though the user was not saved.
    if(!jsonData.has_value() || !jsonData.value().contains("firstName"))
        return std::nullopt;

    QJsonObject automaticLoginData = jsonData.value();
    UserData user {
        automaticLoginData["firstName"].toString(),
        automaticLoginData["lastName"].toString(),
        automaticLoginData["email"].toString(),
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

void AutomaticLoginHelper::addAuthenticationData(
    const AuthenticationData& authData)
{
    auto currentFolder = QDir::current();
    auto automaticLoginFilePath = currentFolder.filePath(m_automaticLoginFile);

    clearAutomaticLoginData();
    QFile file(automaticLoginFilePath);
    if(!file.open(QFile::WriteOnly | QIODevice::Text))
    {
        qDebug() << "...";
        return;
    }

    QJsonObject jsonAuthData {
        { "email", authData.email },
        { "token", authData.token },
    };

    QJsonDocument jsonDoc(jsonAuthData);
    file.write(jsonDoc.toJson((QJsonDocument::Indented)));
}

void AutomaticLoginHelper::addUserData(const UserData& userData)
{
    // User data can only be appended to the file, if the auth data already
    // exists. They would make no sense without each other.
    auto existingAuthData = getAutomaticLoginFileData();
    if(!existingAuthData.has_value())
        return;

    auto currentFolder = QDir::current();
    auto automaticLoginFilePath = currentFolder.filePath(m_automaticLoginFile);
    QFile file(automaticLoginFilePath);
    if(!file.open(QFile::WriteOnly | QIODevice::Text))
    {
        qDebug() << "...";
        return;
    }

    auto jsonAuthData = existingAuthData.value();
    jsonAuthData.insert("firstName", userData.firstName);
    jsonAuthData.insert("lastName", userData.lastName);
    jsonAuthData.insert("email", userData.email);

    // Add tags
    QJsonArray tags;
    for(const auto& tag : userData.tags)
    {
        auto obj = QJsonDocument::fromJson(tag.toJson()).object();
        tags.append(QJsonValue::fromVariant(obj));
    }
    jsonAuthData.insert("tags", tags);

    QJsonDocument jsonDoc(jsonAuthData);
    file.write(jsonDoc.toJson((QJsonDocument::Indented)));
}

void AutomaticLoginHelper::clearAutomaticLoginData()
{
    auto currentFolder = QDir::current();
    auto automaticLoginFilePath = currentFolder.filePath(m_automaticLoginFile);

    QFile file(automaticLoginFilePath);
    file.resize(0);  // Delete all contents of the file
}

std::optional<QJsonObject> AutomaticLoginHelper::getAutomaticLoginFileData()
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

}  // namespace application::utility