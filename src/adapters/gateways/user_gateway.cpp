#include "user_gateway.hpp"
#include "i_authentication_access.hpp"
#include "login_dto.hpp"
#include "register_dto.hpp"


namespace adapters::gateways
{

UserGateway::UserGateway(IAuthenticationAccess* authenticationAccess)
    : m_authenticationAccess(authenticationAccess)
{
    QObject::connect(m_authenticationAccess, &IAuthenticationAccess::authenticationResponseReceived,
                     this, &UserGateway::processAuthenticationResult);
    
    QObject::connect(m_authenticationAccess, &IAuthenticationAccess::userCreationResponseReceived,
                     this, &UserGateway::processUserCreationResult);
}


void UserGateway::authenticateUser(domain::models::LoginModel loginModel)
{
    dtos::LoginDto loginDto { .email = loginModel.email(), .password = loginModel.password() };
    
    m_authenticationAccess->authenticateUser(loginDto);
}

void UserGateway::createUser(domain::models::RegisterModel registerModel)
{
    dtos::RegisterDto registerDto { .firstName = registerModel.firstName(), .lastName = registerModel.lastName(),
                                    .email = registerModel.email(), .password = registerModel.password() };
    
    m_authenticationAccess->createUser(registerDto);
}


void UserGateway::processAuthenticationResult(bool success, QString token)
{
    emit authenticationResultReady(success, token);
}

void UserGateway::processUserCreationResult(bool success, QString failureReason)
{
    emit userCreationResultReady(success, failureReason);
}

} // namespace adapters::gateways