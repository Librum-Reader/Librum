#include "user_service.hpp"


namespace application::services
{

UserService::UserService(IUserStorageGateway* userStorageGateway)
    : m_userStorageGateway(userStorageGateway)
{
}


QString UserService::getFirstName()
{
    return m_firstName;
}

void UserService::setFirstName(const QString& newFirstName)
{
    m_firstName = newFirstName;
}


QString UserService::getLastName()
{
    return m_lastName;
}

void UserService::setLastName(const QString& newLastName)
{
    m_lastName = newLastName;
}


QString UserService::getEmail()
{
    return m_email;
}

void UserService::setEmail(const QString& newEmail)
{
    m_email = newEmail;
}

void UserService::setAuthenticationToken(const QString& token)
{
    m_authenticationToken = token;
}

void UserService::clearAuthenticationToken()
{
    m_authenticationToken.clear();
}

} // namespace application::services