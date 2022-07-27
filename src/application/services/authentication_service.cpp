#include "authentication_service.hpp"
#include <iostream>


namespace application::services
{

AuthenticationService::AuthenticationService(IUserGateway* userGateway)
    : m_userGateway(userGateway)
{
    
}

void AuthenticationService::authenticateUser(QString email, QString password)
{
    std::cout << "Authenticating!" << std::endl;
}

void AuthenticationService::registerUser(QString email, QString password)
{
    
}

} // namespace application::services