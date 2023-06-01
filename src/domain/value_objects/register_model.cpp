#include "register_model.hpp"

namespace domain::value_objects
{

RegisterModel::RegisterModel(QString firstName, QString lastName, QString email,
                             QString password, bool keepUpdated) :
    m_firstName(firstName),
    m_lastName(lastName),
    m_email(email),
    m_password(password),
    m_keepUpdated(keepUpdated)
{
}

QString RegisterModel::getFirstName() const
{
    return m_firstName;
}

QString RegisterModel::getLastName() const
{
    return m_lastName;
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