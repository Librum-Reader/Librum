#include "authentication_service.hpp"
#include "i_user_storage_gateway.hpp"


namespace application::services
{

using namespace domain::models;

AuthenticationService::AuthenticationService(IUserStorageGateway* userStorageGateway)
    : m_userStorageGateway(userStorageGateway)
{
    QObject::connect(m_userStorageGateway, &IUserStorageGateway::authenticationFinished,
                     this, &AuthenticationService::processAuthenticationResult);
    
    QObject::connect(m_userStorageGateway, &IUserStorageGateway::registrationFinished,
                     this, &AuthenticationService::processRegistrationResult);
    
    QObject::connect(this, &AuthenticationService::authenticationTokenRegistered,
                     this, &AuthenticationService::setAuthenticationToken);
}


void AuthenticationService::loginUser(const LoginModel& loginModel)
{
    if(loginModel.isValid())
    {
        m_userStorageGateway->authenticateUser(loginModel);
    }
    else
    {
        emit loginFinished(false);
    }
}

void AuthenticationService::registerUser(const RegisterModel& registerModel)
{
    auto status = registerModel.isValid();
    if(status == RegisterModel::RegistrationResult::Valid)
    {
        m_userStorageGateway->registerUser(registerModel);
    }
    else
    {
        QString failureReason = registerModel.generateErrorMessage(status);
        emit registrationFinished(false, failureReason);
    }
}


void AuthenticationService::processAuthenticationResult(const QString& token)
{
    if(token.isEmpty())
    {
        emit loginFinished(false);
        return;
    }
    
    emit authenticationTokenRegistered(token);
    emit loginFinished(true);
}

void AuthenticationService::processRegistrationResult(bool success,
                                                      const QString& reason)
{
    emit registrationFinished(success, reason);
}

void AuthenticationService::setAuthenticationToken(const QString& token)
{
    m_token = token;
}

} // namespace application::services