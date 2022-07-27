#include "authentication_controller.hpp"
#include <iostream>


namespace adapters::controllers
{

AuthenticationController::AuthenticationController(application::IAuthenticationService* authenticationService)
    : m_authenticationService(authenticationService)
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