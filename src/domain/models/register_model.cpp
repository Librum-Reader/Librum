#include "register_model.hpp"


namespace domain::models
{

RegisterModel::RegisterModel(QString firstName, QString lastName, QString email,
                             QString password, bool keepUpdated)
    : m_firstName(firstName), m_lastName(lastName), m_email(email),
      m_password(password), m_keepUpdated(keepUpdated)
{
    
}


QString RegisterModel::generateErrorMessage(RegistrationResult status) const
{
    switch(status)
    {
    case RegistrationResult::FirstNameTooShort:
        return "First name too short";
        break;
    case RegistrationResult::FirstNameTooLong:
        return "First name too long";
        break;
    case RegistrationResult::LastNameTooShort:
        return "Last name too short";
        break;
    case RegistrationResult::LastNameTooLong:
        return "Last name too long";
        break;
    case RegistrationResult::EmailTooShort:
        return "Email too short";
        break;
    case RegistrationResult::EmailTooLong:
        return "Email too long";
        break;
    case RegistrationResult::PasswordTooShort:
        return "Password too short";
        break;
    case RegistrationResult::PasswordTooLong:
        return "Password too long";
        break;
    default:
        return "Unknown error";
    }
}


QString RegisterModel::firstName() const
{
    return m_firstName;
}

QString RegisterModel::lastName() const
{
    return m_lastName;
}

QString RegisterModel::email() const
{
    return m_email;
}

QString RegisterModel::password() const
{
    return m_password;
}

bool RegisterModel::keepUpdated() const
{
    return m_keepUpdated;
}


RegisterModel::RegistrationResult RegisterModel::isValid() const
{
    auto firstNameStatus = firstNameIsValid();
    if(firstNameStatus != RegistrationResult::Valid)
        return firstNameStatus;
    
    auto lastNameStatus = lastNameIsValid();
    if(lastNameStatus != RegistrationResult::Valid)
        return lastNameStatus;
    
    auto emailStatus = emailIsValid();
    if(emailStatus != RegistrationResult::Valid)
        return emailStatus;
    
    auto passwordStatus = passwordIsValid();
    if(passwordStatus != RegistrationResult::Valid)
        return passwordStatus;
    
    
    return RegistrationResult::Valid;
}

RegisterModel::RegistrationResult RegisterModel::firstNameIsValid() const
{
    if(m_firstName.length() < m_minFirstNameLength)
        return RegistrationResult::FirstNameTooShort;
    if(m_firstName.length() > m_maxFirstNameLength)
        return RegistrationResult::FirstNameTooLong;
    
    return RegistrationResult::Valid;
}

RegisterModel::RegistrationResult RegisterModel::lastNameIsValid() const
{
    if(m_lastName.length() < m_minLastNameLength)
        return RegistrationResult::LastNameTooShort;
    if(m_lastName.length() > m_maxLastNameLength)
        return RegistrationResult::LastNameTooLong;
    
    return RegistrationResult::Valid;
}

RegisterModel::RegistrationResult RegisterModel::emailIsValid() const
{
    if(m_email.length() < m_minEmailLength)
        return RegistrationResult::EmailTooShort;
    if(m_email.length() > m_maxEmailLength)
        return RegistrationResult::EmailTooLong;
    
    return RegistrationResult::Valid;
}

RegisterModel::RegistrationResult RegisterModel::passwordIsValid() const
{
    if(m_password.length() < m_minPasswordLength)
        return RegistrationResult::PasswordTooShort;
    if(m_password.length() > m_maxPasswordLength)
        return RegistrationResult::PasswordTooLong;
    
    return RegistrationResult::Valid;
}

} // namespace domain::models