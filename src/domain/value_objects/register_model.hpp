#pragma once
#include <QString>

namespace domain::value_objects
{

class RegisterModel
{
public:
    RegisterModel(QString firstName, QString lastName, QString email,
                  QString password, bool keepUpdated);

    QString getFirstName() const;
    QString getLastName() const;
    QString getEmail() const;
    QString getPassword() const;
    bool getKeepUpdated() const;

private:
    QString m_firstName;
    QString m_lastName;
    QString m_email;
    QString m_password;
    const bool m_keepUpdated;
};

}  // namespace domain::value_objects