#pragma once
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
 * no data in the "automatic login file" the login process will continue
 * normally.
 */
class AutomaticLoginHelper
{
public:
    std::optional<AuthenticationData> tryAutomaticAuthentication();
    std::optional<UserData> tryAutomaticUserLoading();
    void addAuthenticationData(const AuthenticationData& authData);
    void addUserData(const UserData& userData);
    void clearAutomaticLoginData();

private:
    std::optional<QJsonObject> getAutomaticLoginFileData();

    QString m_automaticLoginFile = "autologin.txt";
};

}  // namespace application::utility