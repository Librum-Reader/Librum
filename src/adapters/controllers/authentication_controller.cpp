#include "authentication_controller.hpp"
#include <iostream>

using namespace application::services;


namespace adapters::controllers
{

AuthenticationController::AuthenticationController(IAuthenticationService* authenticationService)
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