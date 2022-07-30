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
    QString m_email;
    QString m_password;
    int m_minPasswordLength = 6;
};

} // namespace domain::models