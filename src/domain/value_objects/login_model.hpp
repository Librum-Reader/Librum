#pragma once
#include <QString>

namespace domain::value_objects
{

class LoginModel
{
public:
    LoginModel(QString email, QString password, bool rememberUser);

    QString getEmail() const;
    QString getPassword() const;
    bool getRememberUser() const;

private:
    QString m_email;
    QString m_password;
    bool m_rememberUser = false;
};

}  // namespace domain::value_objects