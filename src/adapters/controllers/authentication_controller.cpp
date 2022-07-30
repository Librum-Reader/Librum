#include "authentication_controller.hpp"
#include "i_authentication_service.hpp"
#include "login_model.hpp"


namespace adapters::controllers
{

AuthenticationController::AuthenticationController(application::IAuthenticationService* authenticationService)
    : m_authenticationService(authenticationService)
{
}


bool AuthenticationController::authenticateUser(QString email, QString password)
{
    domain::models::LoginModel loginModel(email, password);
    
    QObject::connect(m_authenticationService, &application::IAuthenticationService::authenticationSucceeded,
                     this, &AuthenticationController::success);
    
    QObject::connect(m_authenticationService, &application::IAuthenticationService::authenticationFailed,
                     this, &AuthenticationController::failure);
    
    return m_authenticationService->authenticateUser(loginModel);
}

void AuthenticationController::success()
{
    emit authenticationSucceeded();
}

void AuthenticationController::failure()
{
    emit authenticationFailed();
}

} // namespace adapters::controllers