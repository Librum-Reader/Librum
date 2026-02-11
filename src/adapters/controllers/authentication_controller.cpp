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
            this,
            [this](ErrorCode code, const QString& message)
            {
                emit loginFinished(static_cast<int>(code), message);
            });

    connect(m_authenticationService,
            &IAuthenticationService::registrationFinished, this,
            [this](ErrorCode code, const QString& message)
            {
                emit registrationFinished(static_cast<int>(code), message);
            });

    connect(m_authenticationService,
            &IAuthenticationService::emailConfirmationCheckFinished, this,
            &IAuthenticationController::emailConfirmationCheckFinished);
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

void AuthenticationController::loginOffline()
{
    m_authenticationService->loginOffline();
}

void AuthenticationController::logoutUser()
{
    m_authenticationService->logoutUser();
}

void AuthenticationController::registerUser(const QString& name,
                                            const QString& email,
                                            QString password, bool keepUpdated)
{
    // Make sure that the email is always lower-case
    auto fixedEmail = email.toLower();
    RegisterModel registerModel(name, fixedEmail, password, keepUpdated);

    m_authenticationService->registerUser(registerModel);
}

void AuthenticationController::checkIfEmailConfirmed(const QString& email)
{
    m_authenticationService->checkIfEmailConfirmed(email);
}

}  // namespace adapters::controllers
