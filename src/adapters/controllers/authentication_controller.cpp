#include "authentication_controller.hpp"
#include "i_authentication_service.hpp"
#include "login_model.hpp"


namespace adapters::controllers
{

AuthenticationController::AuthenticationController(application::IAuthenticationService* authenticationService)
    : m_authenticationService(authenticationService)
{
    QObject::connect(m_authenticationService, &application::IAuthenticationService::succeeded,
                     this, &AuthenticationController::success);
    QObject::connect(m_authenticationService, &application::IAuthenticationService::failed,
                     this, &AuthenticationController::failure);
}


bool AuthenticationController::loginUser(QString email, QString password)
{
    domain::models::LoginModel loginModel(email, password);
    
    return m_authenticationService->authenticateUser(loginModel);
}

bool AuthenticationController::registerUser(QString firstName, QString lastName, QString email, 
                                            QString password, bool keepUpdated)
{
    
}

void AuthenticationController::success()
{
    emit loginSucceeded();
}

void AuthenticationController::failure()
{
    emit loginFailed();
}

} // namespace adapters::controllers