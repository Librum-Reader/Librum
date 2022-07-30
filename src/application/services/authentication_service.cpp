#include "authentication_service.hpp"
#include "i_user_gateway.hpp"


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
    
    QObject::connect(m_userGateway, &IUserGateway::resultReady, this, &AuthenticationService::processResult);
    
    m_userGateway->loginUser(loginModel);
    return true;
}

void AuthenticationService::processResult(QString token)
{
    // Store result
    
    if(token.isEmpty())
    {
        emit authenticationFailed();
        return;
    }
    
    emit authenticationSucceeded();
}

} // namespace application::services