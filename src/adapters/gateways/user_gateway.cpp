#include "user_gateway.hpp"
#include "i_authentication_access.hpp"
#include "login_dto.hpp"


namespace adapters::gateways
{

UserGateway::UserGateway(IAuthenticationAccess* authenticationAccess)
    : m_authenticationAccess(authenticationAccess)
{
    
}

void UserGateway::loginUser(domain::models::LoginModel loginModel)
{
    dtos::LoginDto loginDto { .email = loginModel.email(), .password = loginModel.password() };
    
    QObject::connect(m_authenticationAccess, &IAuthenticationAccess::requestFinished, this, &UserGateway::processToken);
    m_authenticationAccess->loginUser(loginDto);
}

void UserGateway::processToken(QString token)
{
    emit resultReady(token);
}


} // namespace adapters::gateways