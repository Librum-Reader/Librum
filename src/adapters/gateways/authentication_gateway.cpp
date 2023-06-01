#include "authentication_gateway.hpp"
#include "api_error_codes.hpp"


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
                auto errorCode = static_cast<ApiErrorCodes>(code);
                emit authenticationFinished(token, errorCode);
            });

    connect(m_authenticationAccess,
            &IAuthenticationAccess::registrationFinished, this,
            [this](int code)
            {
                auto errorCode = static_cast<ApiErrorCodes>(code);
                emit registrationFinished(errorCode);
            });
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
        .firstName = registerModel.getFirstName(),
        .lastName = registerModel.getLastName(),
        .email = registerModel.getEmail(),
        .password = registerModel.getPassword(),
    };

    m_authenticationAccess->registerUser(registerDto);
}

}  // namespace adapters::gateways