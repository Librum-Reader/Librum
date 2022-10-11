#include "user_storage_gateway.hpp"


using namespace domain::models;

namespace adapters::gateways
{

UserStorageGateway::UserStorageGateway(IUserStorageAccess* userStorageAccess)
    : m_userStorageAccess(userStorageAccess)
{
    QObject::connect(m_userStorageAccess, &IUserStorageAccess::userReady,
                     this, &UserStorageGateway::proccessUserData);
    
    QObject::connect(m_userStorageAccess, &IUserStorageAccess::gettingUserFailed,
                     this, &UserStorageGateway::reportFailureGettingUser);
}

void UserStorageGateway::getUser(const QString& authenticationToken)
{
    m_userStorageAccess->getUser(authenticationToken);
}

void UserStorageGateway::proccessUserData(const QString& firstName, 
                                          const QString& lastName,
                                          const QString& email)
{
    User user;
    user.setFirstName(firstName);
    user.setLastName(lastName);
    user.setEmail(email);
    
    emit finishedGettingUser(user, true);
}

void UserStorageGateway::reportFailureGettingUser()
{
    User emptyUser;
    emit finishedGettingUser(emptyUser, false);
}

} // namespace adapters::gateways