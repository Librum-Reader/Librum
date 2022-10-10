#include "authentication_gateway.hpp"


namespace adapters::gateways
{

AuthenticationGateway::AuthenticationGateway(IAuthenticationAccess* authenticationAccess)
    : m_authenticationAccess(authenticationAccess)
{
    QObject::connect(m_authenticationAccess, &IAuthenticationAccess::authenticationFinished,
                     this, &AuthenticationGateway::reemitAuthenticationResult);
    
    QObject::connect(m_authenticationAccess, &IAuthenticationAccess::registrationFinished,
                     this, &AuthenticationGateway::reemitRegistrationResult);
}


void AuthenticationGateway::authenticateUser(const domain::models::LoginModel& loginModel)
{
    dtos::LoginDto loginDto
    {
        .email = loginModel.getEmail(),
        .password = loginModel.getPassword()
    };
    
    m_authenticationAccess->authenticateUser(loginDto);
}

void AuthenticationGateway::registerUser(const domain::models::RegisterModel& registerModel)
{
    dtos::RegisterDto registerDto
    {
        .firstName = registerModel.getFirstName(),
        .lastName = registerModel.getLastName(),
        .email = registerModel.getEmail(),
        .password = registerModel.getPassword()
    };
    
    m_authenticationAccess->registerUser(registerDto);
}

void AuthenticationGateway::reemitRegistrationResult(bool success, const QString& reason)
{
    emit registrationFinished(success, reason);
}

void AuthenticationGateway::reemitAuthenticationResult(const QString& token)
{
    emit authenticationFinished(token);
}




} // namespace adapters::gateways