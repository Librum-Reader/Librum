#pragma once
#include <QString>

namespace domain::value_objects
{

class LoginModel
{
public:
    LoginModel(QString email, QString password, bool rememberMe);

    QString getEmail() const;
    QString getPassword() const;
    bool getRememberMe() const;
    bool isValid() const;

private:
    bool emailIsValid() const;
    bool passwordIsValid() const;

    QString m_email;
    const int m_minEmailLength = 6;
    const int m_maxEmailLength = 50;
    QString m_password;
    const int m_minPasswordLength = 6;
    const int m_maxPasswordLength = 60;
    bool m_rememberMe = false;
};

}  // namespace domain::value_objects