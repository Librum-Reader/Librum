#include "user_storage_gateway.hpp"


using namespace domain::models;

namespace adapters::gateways
{

UserStorageGateway::UserStorageGateway(IUserStorageAccess* userStorageAccess)
    : m_userStorageAccess(userStorageAccess)
{
    connect(m_userStorageAccess, &IUserStorageAccess::userReady,
            this, &UserStorageGateway::proccessUserData);
    
    connect(m_userStorageAccess, &IUserStorageAccess::gettingUserFailed,
            this, &UserStorageGateway::reportFailureGettingUser);
}


void UserStorageGateway::getUser(const QString& authToken)
{
    m_userStorageAccess->getUser(authToken);
}

void UserStorageGateway::changeFirstName(const QString& authToken,
                                         const QString& newFirstName)
{
    m_userStorageAccess->changeFirstName(authToken, newFirstName);
}

void UserStorageGateway::changeLastName(const QString& authToken,
                                        const QString& newLastName)
{
    m_userStorageAccess->changeLastName(authToken, newLastName);
}

void UserStorageGateway::changeEmail(const QString& authToken,
                                     const QString& newEmail)
{
    m_userStorageAccess->changeEmail(authToken, newEmail);
}

void UserStorageGateway::proccessUserData(const QString& firstName, 
                                          const QString& lastName,
                                          const QString& email)
{
    User user (firstName, lastName, email);
    emit finishedGettingUser(user, true);
}

void UserStorageGateway::reportFailureGettingUser()
{
    User emptyUser;
    emit finishedGettingUser(emptyUser, false);
}

} // namespace adapters::gateways