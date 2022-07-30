#include "authentication_controller.hpp"
#include "login_model.hpp"


namespace adapters::controllers
{

AuthenticationController::AuthenticationController(application::IAuthenticationService* authenticationService)
    : m_authenticationService(authenticationService)
{
}

void AuthenticationController::registerUser()
{
}

bool AuthenticationController::authenticateUser(QString email, QString password)
{
    domain::models::LoginModel loginModel(email, password);
    
    return m_authenticationService->authenticateUser(loginModel);
}

} // namespace adapters::controllers