#include "login_model.hpp"

namespace domain::value_objects
{

LoginModel::LoginModel(QString email, QString password, bool rememberUser) :
    m_email(email),
    m_password(password),
    m_rememberUser(rememberUser)
{
}

QString LoginModel::getEmail() const
{
    return m_email;
}

QString LoginModel::getPassword() const
{
    return m_password;
}

bool LoginModel::getRememberUser() const
{
    return m_rememberUser;
}

}  // namespace domain::value_objects