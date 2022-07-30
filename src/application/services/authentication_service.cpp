#include "authentication_service.hpp"


namespace application::services
{

AuthenticationService::AuthenticationService(IUserGateway* userGateway)
    : m_userGateway(userGateway)
{
    
}

bool AuthenticationService::authenticateUser(domain::models::LoginModel loginModel)
{
    if(!loginModel.isValid())
        return false;
    
    QString token = m_userGateway->loginUser(loginModel);
    return !token.isEmpty();
}

void AuthenticationService::registerUser(QString email, QString password)
{
    
}

} // namespace application::services