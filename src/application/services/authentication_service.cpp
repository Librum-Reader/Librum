#include "authentication_service.hpp"
#include "i_user_gateway.hpp"


namespace application::services
{

AuthenticationService::AuthenticationService(IUserGateway* userGateway)
    : m_userGateway(userGateway)
{
    QObject::connect(m_userGateway, &IUserGateway::resultReady, this, &AuthenticationService::processResult);
}

bool AuthenticationService::authenticateUser(domain::models::LoginModel loginModel)
{
    if(!loginModel.isValid())
        return false;
    
    m_userGateway->authenticateUser(loginModel);
    return true;
}

void AuthenticationService::processResult(QString token)
{
    if(token.isEmpty())
    {
        emit failed();
        return;
    }
    
    emit succeeded();
}

} // namespace application::services