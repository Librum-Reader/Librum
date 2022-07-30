#include "authentication_service.hpp"
#include "i_user_gateway.hpp"


namespace application::services
{

AuthenticationService::AuthenticationService(IUserGateway* userGateway)
    : m_userGateway(userGateway)
{
    QObject::connect(m_userGateway, &IUserGateway::authenticationResultReady,
                     this, &AuthenticationService::processLoginResult);
    
    QObject::connect(m_userGateway, &IUserGateway::userCreationResultReady,
                     this, &AuthenticationService::processRegistrationResult);
}

bool AuthenticationService::authenticateUser(domain::models::LoginModel loginModel)
{
    if(!loginModel.isValid())
        return false;
    
    m_userGateway->authenticateUser(loginModel);
    return true;
}

bool AuthenticationService::registerUser(domain::models::RegisterModel registerModel)
{
    if(!registerModel.isValid())
        return false;
    
    m_userGateway->createUser(registerModel);
    return true;
}

void AuthenticationService::processLoginResult(bool success, QString token)
{
    if(!success)
    {
        emit authenticationFailed();
        return;
    }
    
    emit authenticationSucceeded();
}

void AuthenticationService::processRegistrationResult(bool success, QString failureReason)
{
    if(!success)
    {
        emit registrationFailed(failureReason);
        return;
    }
    
    emit registrationSucceeded();
}

} // namespace application::services