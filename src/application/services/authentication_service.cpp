#include "authentication_service.hpp"
#include <QDebug>
#include "api_error_code_converter.hpp"
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
                utility::AutomaticLoginHelper::clearAutomaticLoginData();
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
    auto result = utility::AutomaticLoginHelper::tryAutomaticAuthentication();
    if(!result.has_value())
        return;

    utility::AuthenticationData authData = result.value();
    m_tempEmail = authData.email;

    processAuthenticationResult(authData.token, ApiErrorCodes::NoError);
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

void AuthenticationService::processAuthenticationResult(const QString& token,
                                                        ApiErrorCodes errorCode)
{
    if(errorCode != ApiErrorCodes::NoError)
    {
        auto errorMessage =
            utility::ApiErrorCodeConverter::getMessageForErrorCode(errorCode);

        qWarning() << errorMessage;
        emit loginFinished(false, errorMessage);
    }
    else
    {
        // Store the login data if "rememberUser" was selected
        if(m_rememberUser)
        {
            utility::AuthenticationData authData { m_tempEmail, token };
            utility::AutomaticLoginHelper::saveAuthenticationData(authData);
        }

        emit loggedIn(token, m_tempEmail);
        emit loginFinished(true);
    }

    clearTemporaryUserData();
}

void AuthenticationService::processRegistrationResult(ApiErrorCodes errorCode)
{
    bool success = errorCode == ApiErrorCodes::NoError;
    QString errorMessage;
    if(!success)
    {
        errorMessage =
            utility::ApiErrorCodeConverter::getMessageForErrorCode(errorCode);
    }

    emit registrationFinished(success, errorMessage);
}

void AuthenticationService::clearTemporaryUserData()
{
    m_tempEmail = "";
    m_rememberUser = false;
}

}  // namespace application::services