#include "authentication_gateway.hpp"
#include "error_code.hpp"


using namespace domain::value_objects;

namespace adapters::gateways
{

AuthenticationGateway::AuthenticationGateway(
    IAuthenticationAccess* authenticationAccess) :
    m_authenticationAccess(authenticationAccess)
{
    connect(m_authenticationAccess,
            &IAuthenticationAccess::authenticationFinished, this,
            [this](const QString& token, int code)
            {
                auto errorCode = static_cast<ErrorCode>(code);
                emit authenticationFinished(token, errorCode);
            });

    connect(m_authenticationAccess,
            &IAuthenticationAccess::registrationFinished, this,
            [this](int code)
            {
                auto errorCode = static_cast<ErrorCode>(code);
                emit registrationFinished(errorCode);
            });

    connect(m_authenticationAccess,
            &IAuthenticationAccess::emailConfirmationCheckFinished, this,
            &AuthenticationGateway::emailConfirmationCheckFinished);
}

void AuthenticationGateway::authenticateUser(const LoginModel& loginModel)
{
    dtos::LoginDto loginDto {
        .email = loginModel.getEmail(),
        .password = loginModel.getPassword(),
    };

    m_authenticationAccess->authenticateUser(loginDto);
}

void AuthenticationGateway::registerUser(const RegisterModel& registerModel)
{
    dtos::RegisterDto registerDto {
        .name = registerModel.getName(),
        .email = registerModel.getEmail(),
        .password = registerModel.getPassword(),
    };

    m_authenticationAccess->registerUser(registerDto);
}

void AuthenticationGateway::checkIfEmailConfirmed(const QString& email)
{
    m_authenticationAccess->checkIfEmailConfirmed(email);
}

}  // namespace adapters::gateways