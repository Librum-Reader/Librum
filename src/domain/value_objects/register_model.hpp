#pragma once
#include <QString>

namespace domain::value_objects
{

class RegisterModel
{
public:
    RegisterModel(QString firstName, QString lastName, QString email,
                  QString password, bool keepUpdated);


    enum class RegistrationResult
    {
        Valid,
        FirstNameTooShort,
        FirstNameTooLong,
        LastNameTooShort,
        LastNameTooLong,
        EmailTooShort,
        EmailTooLong,
        PasswordTooShort,
        PasswordTooLong
    };

    QString generateErrorMessage(RegistrationResult status) const;
    RegistrationResult isValid() const;

    QString getFirstName() const;
    QString getLastName() const;
    QString getEmail() const;
    QString getPassword() const;
    bool getKeepUpdated() const;

private:
    RegisterModel::RegistrationResult firstNameIsValid() const;
    RegisterModel::RegistrationResult lastNameIsValid() const;
    RegisterModel::RegistrationResult emailIsValid() const;
    RegisterModel::RegistrationResult passwordIsValid() const;

    QString m_firstName;
    const int m_minFirstNameLength = 2;
    const int m_maxFirstNameLength = 40;

    QString m_lastName;
    const int m_minLastNameLength = 2;
    const int m_maxLastNameLength = 50;

    QString m_email;
    const int m_minEmailLength = 6;
    const int m_maxEmailLength = 50;

    QString m_password;
    const int m_minPasswordLength = 6;
    const int m_maxPasswordLength = 60;

    const bool m_keepUpdated;
};

}  // namespace domain::value_objects