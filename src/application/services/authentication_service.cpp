#include "authentication_service.hpp"
#include "i_user_storage_gateway.hpp"


namespace application::services
{

AuthenticationService::AuthenticationService(IUserStorageGateway* userGateway)
    : m_userGateway(userGateway)
{
    QObject::connect(m_userGateway, &IUserStorageGateway::authenticationResultReady,
                     this, &AuthenticationService::processLoginResult);
    
    QObject::connect(m_userGateway, &IUserStorageGateway::userCreationResultReady,
                     this, &AuthenticationService::processRegistrationResult);
}

void AuthenticationService::authenticateUser(domain::models::LoginModel loginModel)
{
    if(!loginModel.isValid())
    {
        emit authenticationFailed();
        return;
    }
    
    m_userGateway->authenticateUser(loginModel);
}

void AuthenticationService::registerUser(domain::models::RegisterModel registerModel)
{
    if(!registerModel.isValid())
    {
        emit registrationFailed("The provided data is invalid");
        return;
    }
    
    m_userGateway->createUser(registerModel);
}

void AuthenticationService::processLoginResult(bool success, QString token)
{
    Q_UNUSED(token);
    
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