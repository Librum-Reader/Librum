#include "authentication_controller.hpp"
#include <iostream>


namespace adapters::controllers
{

AuthenticationController::AuthenticationController(application::services::IAuthenticationService* authenticationService)
    : m_authenticationService(authenticationService)
{
}

AuthenticationController::~AuthenticationController()
{
    
}

void AuthenticationController::registerUser()
{
}

void AuthenticationController::authenticateUser()
{
    m_authenticationService->authenticateUser("TestEmail", "TestPassword");
}

} // namespace adapters::controllers