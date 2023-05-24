#include "authentication_controller.hpp"
#include "i_authentication_service.hpp"
#include "login_model.hpp"
#include "register_model.hpp"


using namespace application;
using namespace domain::value_objects;

namespace adapters::controllers
{

AuthenticationController::AuthenticationController(
    IAuthenticationService* authenticationService) :
    m_authenticationService(authenticationService)
{
    connect(m_authenticationService, &IAuthenticationService::loginFinished,
            this, &AuthenticationController::loginFinished);

    connect(m_authenticationService,
            &IAuthenticationService::registrationFinished, this,
            &AuthenticationController::registrationFinished);
}

void AuthenticationController::loginUser(const QString& email,
                                         const QString& password,
                                         bool rememberUser)
{
    // Make sure that the email is always lower-case
    auto fixedEmail = email.toLower();
    LoginModel loginModel(fixedEmail, password, rememberUser);

    m_authenticationService->loginUser(loginModel);
}

void AuthenticationController::tryAutomaticLogin()
{
    m_authenticationService->tryAutomaticLogin();
}

void AuthenticationController::logoutUser()
{
    m_authenticationService->logoutUser();
}

void AuthenticationController::registerUser(const QString& firstName,
                                            const QString& lastName,
                                            const QString& email,
                                            QString password, bool keepUpdated)
{
    // Make sure that the email is always lower-case
    auto fixedEmail = email.toLower();
    RegisterModel registerModel(firstName, lastName, fixedEmail, password,
                                 keepUpdated);

    m_authenticationService->registerUser(registerModel);
}

}  // namespace adapters::controllers
