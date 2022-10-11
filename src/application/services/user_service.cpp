#include "user_service.hpp"


namespace application::services
{

UserService::UserService(IUserStorageGateway* userStorageGateway)
    : m_userStorageGateway(userStorageGateway)
{
    m_user.setFirstName("Alex");
    m_user.setLastName("Manson");
    m_user.setEmail("alexmenson@librum.com");
}


QString UserService::getFirstName()
{
    return m_user.getFirstName();
}

void UserService::setFirstName(const QString& newFirstName)
{
    m_user.setFirstName(newFirstName);
}


QString UserService::getLastName()
{
    return m_user.getLastName();
}

void UserService::setLastName(const QString& newLastName)
{
    m_user.setLastName(newLastName);
}


QString UserService::getEmail()
{
    return m_user.getEmail();
}

void UserService::setEmail(const QString& newEmail)
{
    m_user.setEmail(newEmail);
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