#include "register_model.hpp"


namespace domain::models
{

RegisterModel::RegisterModel(QString firstName, QString lastName, QString email,
                             QString password, bool keepUpdated)
    : m_firstName(firstName), m_lastName(lastName), m_email(email),
      m_password(password), m_keepUpdated(keepUpdated)
{
    
}


bool RegisterModel::isValid() const
{
    if(firstNameIsValid() && lastNameIsValid() && emailIsValid() && passwordIsValid())
        return true;
    
    return false;
}

QString RegisterModel::firstName()
{
    return m_firstName;
}

QString RegisterModel::lastName()
{
    return m_lastName;
}

QString RegisterModel::email()
{
    return m_email;
}

QString RegisterModel::password()
{
    return m_password;
}

bool RegisterModel::keepUpdated()
{
    return m_keepUpdated;
}


bool RegisterModel::firstNameIsValid() const
{
    return m_firstName.length() <= m_maxFirstNameLength && m_firstName.length() >= m_minFirstNameLength;
}

bool RegisterModel::lastNameIsValid() const
{
    return m_lastName.length() <= m_maxLastNameLength && m_lastName.length() >= m_minLastNameLength;
}

bool RegisterModel::emailIsValid() const
{
    return m_email.length() <= m_maxEmailLength && m_email.length() >= m_minEmailLength;
}

bool RegisterModel::passwordIsValid() const
{
    return m_password.length() <= m_maxPasswordLength && m_password.length() >= m_minPasswordLength;
}

} // namespace domain::models