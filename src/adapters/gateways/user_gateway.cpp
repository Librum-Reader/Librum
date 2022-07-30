#include "user_gateway.hpp"
#include "login_dto.hpp"


namespace adapters::gateways
{

UserGateway::UserGateway(IAuthenticationAccess* authenticationAccess)
    : m_authenticationAccess(authenticationAccess)
{
    
}

QString UserGateway::loginUser(domain::models::LoginModel loginModel)
{
    dtos::LoginDto loginDto { .email = loginModel.email(), .password = loginModel.password() };
    
    return m_authenticationAccess->loginUser(loginDto);
}

} // namespace adapters::gateways