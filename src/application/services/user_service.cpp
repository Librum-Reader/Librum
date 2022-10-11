#include "user_service.hpp"


namespace application::services
{

UserService::UserService(IUserStorageGateway* userStorageGateway)
    : m_userStorageGateway(userStorageGateway)
{
    QObject::connect(m_userStorageGateway, &IUserStorageGateway::finishedGettingUser,
                     this, &UserService::proccessUserInformation);
}


void UserService::loadUser()
{
    m_userStorageGateway->getUser(m_authenticationToken);
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

void UserService::proccessUserInformation(const domain::models::User& user,
                                          bool success)
{
    if(!success)
        emit finishedLoadingUser(false);
    
    m_user.setFirstName(user.getFirstName());
    m_user.setLastName(user.getLastName());
    m_user.setEmail(user.getEmail());
    
    finishedLoadingUser(true);
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