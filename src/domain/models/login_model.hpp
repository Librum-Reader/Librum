#pragma once
#include <QString>


namespace domain::models
{

class LoginModel
{
public:
    LoginModel(QString email, QString password);
    
    QString email();
    QString password();
    bool isValid() const;
    
private:
    bool emailIsValid() const;
    bool passwordIsValid() const;
    
private:
    QString m_email;
    int m_minEmailLength = 6;
    int m_maxEmailLength = 50;
    
    QString m_password;
    int m_minPasswordLength = 6;
    int m_maxPasswordLength = 60;
};

} // namespace domain::models