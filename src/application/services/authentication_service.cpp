#include "authentication_service.hpp"
#include <QDebug>
#include "automatic_login_helper.hpp"
#include "error_code_converter.hpp"
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

    processAuthenticationResult(authData.token, ErrorCode::NoError);
}

void AuthenticationService::logoutUser()
{
    emit loggedOut();
}

void AuthenticationService::registerUser(const RegisterModel& registerModel)
{
    m_authenticationGateway->registerUser(registerModel);
}

void AuthenticationService::processAuthenticationResult(const QString& token,
                                                        ErrorCode errorCode)
{
    if(errorCode != ErrorCode::NoError)
    {
        auto errorMessage =
            utility::error_code_converter::getMessageForErrorCode(errorCode);

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

void AuthenticationService::processRegistrationResult(ErrorCode errorCode)
{
    bool success = errorCode == ErrorCode::NoError;
    QString errorMessage;
    if(!success)
    {
        errorMessage =
            utility::error_code_converter::getMessageForErrorCode(errorCode);
    }

    emit registrationFinished(success, errorMessage);
}

void AuthenticationService::clearTemporaryUserData()
{
    m_tempEmail = "";
    m_rememberUser = false;
}

}  // namespace application::services