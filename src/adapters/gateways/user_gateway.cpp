#include "user_gateway.hpp"
#include "login_dto.hpp"


namespace adapters::gateways
{

UserGateway::UserGateway(IAuthenticationAccess* authenticationAccess)
    : m_authenticationAccess(authenticationAccess)
{
    
}

void UserGateway::loginUser(QString email, QString password)
{
    dtos::LoginDto loginDto { .email = email, .password = password };
    m_authenticationAccess->loginUser(loginDto);
}

} // namespace adapters::gateways