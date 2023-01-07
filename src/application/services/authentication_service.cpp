#include "authentication_service.hpp"
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
}

void AuthenticationService::loginUser(const LoginModel& loginModel)
{
    if(!loginModel.isValid())
    {
        emit loginFinished(false);
        return;
    }

    m_tempEmail = loginModel.getEmail();
    m_authenticationGateway->authenticateUser(loginModel);
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
        emit registrationFinished(false, failureReason);
        return;
    }

    m_authenticationGateway->registerUser(registerModel);
}

void AuthenticationService::processAuthenticationResult(const QString& token)
{
    if(token.isEmpty())
    {
        emit loginFinished(false);
        return;
    }

    emit loggedIn(token, m_tempEmail);
    emit loginFinished(true);

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
}

}  // namespace application::services