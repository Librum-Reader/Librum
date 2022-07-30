#include "login_model.hpp"


namespace domain::models
{

LoginModel::LoginModel(QString email, QString password)
    : m_email(email), m_password(password)
{
}

QString LoginModel::email()
{
    return m_email;
}

QString LoginModel::password()
{
    return m_password;
}

bool LoginModel::isValid() const
{
    if(m_password.isEmpty() || m_password.isNull() || m_email.isEmpty() || m_email.isNull())
        return false;
    
    if(m_password.length() < m_minPasswordLength)
        return false;
    
    return true;
}

} // namespace domain::models