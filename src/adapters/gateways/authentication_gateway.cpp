#include "authentication_gateway.hpp"


namespace adapters::gateways
{

AuthenticationGateway::AuthenticationGateway(IAuthenticationAccess* authenticationAccess)
    : m_authenticationAccess(authenticationAccess)
{
    connect(m_authenticationAccess, &IAuthenticationAccess::authenticationFinished,
            this, &AuthenticationGateway::authenticationFinished);
    
    connect(m_authenticationAccess, &IAuthenticationAccess::registrationFinished,
            this, &AuthenticationGateway::registrationFinished);
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

} // namespace adapters::gateways