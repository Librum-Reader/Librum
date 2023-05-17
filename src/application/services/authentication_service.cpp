#include "authentication_service.hpp"
#include <QDebug>
#include "automatic_login_helper.hpp"
#include "i_authentication_gateway.hpp"


using namespace domain::value_objects;

namespace application::services
{

AuthenticationService::AuthenticationService(
    IAuthenticationGateway* authenticationGateway) :
    m_authenticationGateway(authenticationGateway)
{
    connect(m_authenticationGateway,
            &IAuthenticationGateway::authenticationFinished, this,
            &AuthenticationService::processAuthenticationResult);

    connect(m_authenticationGateway,
            &IAuthenticationGateway::registrationFinished, this,
            &AuthenticationService::processRegistrationResult);

    // Delete the user data when logging out
    connect(this, &AuthenticationService::loggedOut, this,
            []()
            {
                utility::AutomaticLoginHelper autoLoginHelper;
                autoLoginHelper.clearAutomaticLoginData();
            });
}

void AuthenticationService::loginUser(const LoginModel& loginModel)
{
    if(!loginModel.isValid())
    {
        qWarning() << "Login failed due to invalid login model";

        emit loginFinished(false);
        return;
    }

    m_rememberUser = loginModel.getRememberUser();
    m_tempEmail = loginModel.getEmail();
    m_authenticationGateway->authenticateUser(loginModel);
}

void AuthenticationService::tryAutomaticLogin()
{
    utility::AutomaticLoginHelper autoLoginHelper;
    auto result = autoLoginHelper.tryAutomaticAuthentication();
    if(!result.has_value())
        return;

    utility::AuthenticationData authData = result.value();
    m_tempEmail = authData.email;

    processAuthenticationResult(authData.token);
}

void AuthenticationService::logoutUser()
{
    emit loggedOut();
}

void AuthenticationService::registerUser(const RegisterModel& registerModel)
{
    auto status = registerModel.isValid();
    if(status != RegisterModel::RegistrationResult::Valid)
    {
        QString failureReason = registerModel.generateErrorMessage(status);
        qWarning() << QString("Failed registration: %1").arg(failureReason);
        emit registrationFinished(false, failureReason);
        return;
    }

    m_authenticationGateway->registerUser(registerModel);
}

void AuthenticationService::processAuthenticationResult(const QString& token)
{
    if(token.isEmpty())
    {
        qWarning() << "Authentication token is empty";
        emit loginFinished(false);
    }
    else
    {
        // Store the login data if "rememberUser" was selected
        if(m_rememberUser)
        {
            utility::AutomaticLoginHelper autoLoginHelper;
            utility::AuthenticationData authData { m_tempEmail, token };
            autoLoginHelper.saveAuthenticationData(authData);
        }

        emit loggedIn(token, m_tempEmail);
        emit loginFinished(true);
    }

    clearTemporaryUserData();
}

void AuthenticationService::processRegistrationResult(bool success,
                                                      const QString& reason)
{
    emit registrationFinished(success, reason);
}

void AuthenticationService::clearTemporaryUserData()
{
    m_tempEmail = "";
    m_rememberUser = false;
}

}  // namespace application::services