#pragma once
#include <QString>

namespace domain::value_objects
{

class LoginModel
{
public:
    LoginModel(QString email, QString password);

    QString getEmail() const;
    QString getPassword() const;
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
};

}  // namespace domain::value_objects