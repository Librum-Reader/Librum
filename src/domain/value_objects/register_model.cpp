#include "register_model.hpp"

namespace domain::value_objects
{

RegisterModel::RegisterModel(QString name, QString email, QString password,
                             bool keepUpdated) :
    m_name(name),
    m_email(email),
    m_password(password),
    m_keepUpdated(keepUpdated)
{
}

QString RegisterModel::getName() const
{
    return m_name;
}

QString RegisterModel::getEmail() const
{
    return m_email;
}

QString RegisterModel::getPassword() const
{
    return m_password;
}

bool RegisterModel::getKeepUpdated() const
{
    return m_keepUpdated;
}

}  // namespace domain::value_objects