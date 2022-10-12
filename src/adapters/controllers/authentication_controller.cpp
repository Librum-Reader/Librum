#include "authentication_controller.hpp"
#include "i_authentication_service.hpp"
#include "login_model.hpp"
#include "register_model.hpp"


using namespace application;

namespace adapters::controllers
{

AuthenticationController::AuthenticationController(IAuthenticationService* 
                                                   authenticationService)
    : m_authenticationService(authenticationService)
{
    connect(m_authenticationService, &IAuthenticationService::loginFinished,
            this, &AuthenticationController::loginFinished);
    
    connect(m_authenticationService, &IAuthenticationService::registrationFinished,
            this, &AuthenticationController::registrationFinished);
}


void AuthenticationController::loginUser(const QString& email, 
                                         const QString& password)
{
    domain::models::LoginModel loginModel(email, password);
    
    m_authenticationService->loginUser(loginModel);
}

void AuthenticationController::registerUser(const QString& firstName,
                                            const QString& lastName, 
                                            const QString& email,
                                            QString password, bool keepUpdated)
{
    domain::models::RegisterModel registerModel(firstName, lastName, 
                                                email, password, keepUpdated);
    
    m_authenticationService->registerUser(registerModel);
}

} // namespace adapters::controllers