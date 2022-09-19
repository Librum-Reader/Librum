#pragma once
#include <QString>


namespace domain::models
{

class RegisterModel
{
public:
    RegisterModel(QString firstName, QString lastName, QString email,
                  QString password, bool keepUpdated);
    
    bool isValid() const;
    
    QString firstName() const;
    QString lastName() const;
    QString email() const;
    QString password() const;
    bool keepUpdated() const;
    
private:
    bool firstNameIsValid() const;
    bool lastNameIsValid() const;
    bool emailIsValid() const;
    bool passwordIsValid() const;
    
private:
    QString m_firstName;
    int m_minFirstNameLength = 2;
    int m_maxFirstNameLength = 40;
    
    QString m_lastName;
    int m_minLastNameLength = 2;
    int m_maxLastNameLength = 50;
    
    QString m_email;
    int m_minEmailLength = 6;
    int m_maxEmailLength = 50;
    
    QString m_password;
    int m_minPasswordLength = 6;
    int m_maxPasswordLength = 60;
    
    bool m_keepUpdated;
};

} // namespace domain::models