#include "user_gateway.hpp"
#include "i_authentication_access.hpp"
#include "login_dto.hpp"


namespace adapters::gateways
{

UserGateway::UserGateway(IAuthenticationAccess* authenticationAccess)
    : m_authenticationAccess(authenticationAccess)
{
    QObject::connect(m_authenticationAccess, &IAuthenticationAccess::responseReceived, this, &UserGateway::processToken);
}

void UserGateway::authenticateUser(domain::models::LoginModel loginModel)
{
    dtos::LoginDto loginDto { .email = loginModel.email(), .password = loginModel.password() };
    
    m_authenticationAccess->authenticateUser(loginDto);
}

void UserGateway::processToken(QString token)
{
    emit resultReady(token);
}


} // namespace adapters::gateways