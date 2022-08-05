#include "authentication_controller.hpp"
#include "i_authentication_service.hpp"
#include "login_model.hpp"
#include "register_model.hpp"


namespace adapters::controllers
{

AuthenticationController::AuthenticationController(application::IAuthenticationService* authenticationService)
    : m_authenticationService(authenticationService)
{
    QObject::connect(m_authenticationService, &application::IAuthenticationService::authenticationSucceeded,
                     this, &AuthenticationController::loginSuccess);
    QObject::connect(m_authenticationService, &application::IAuthenticationService::authenticationFailed,
                     this, &AuthenticationController::loginFailure);
    
    QObject::connect(m_authenticationService, &application::IAuthenticationService::registrationSucceeded,
                     this, &AuthenticationController::registrationSuccess);
    QObject::connect(m_authenticationService, &application::IAuthenticationService::registrationFailed,
                     this, &AuthenticationController::registrationFailure);
}


void AuthenticationController::loginUser(QString email, QString password)
{
    domain::models::LoginModel loginModel(email, password);
    
    m_authenticationService->loginUser(loginModel);
}

void AuthenticationController::registerUser(QString firstName, QString lastName, QString email, 
                                            QString password, bool keepUpdated)
{
    domain::models::RegisterModel registerModel(firstName, lastName, email, password, keepUpdated);
    
    m_authenticationService->registerUser(registerModel);
}


void AuthenticationController::loginSuccess()
{
    emit loginSucceeded();
}

void AuthenticationController::loginFailure()
{
    emit loginFailed();
}

void AuthenticationController::registrationSuccess()
{
    emit registrationSucceeded();
}

void AuthenticationController::registrationFailure(QString reason)
{
    emit registrationFailed(reason);
}

} // namespace adapters::controllers