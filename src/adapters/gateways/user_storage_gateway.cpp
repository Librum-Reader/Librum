#include "user_storage_gateway.hpp"
#include "i_user_storage_access.hpp"
#include "login_dto.hpp"
#include "register_dto.hpp"


namespace adapters::gateways
{

UserStorageGateway::UserStorageGateway(IUserStorageAccess* authenticationAccess)
    : m_authenticationAccess(authenticationAccess)
{
    QObject::connect(m_authenticationAccess, &IUserStorageAccess::authenticationResponseArrived,
                     this, &UserStorageGateway::processAuthenticationResult);
    
    QObject::connect(m_authenticationAccess, &IUserStorageAccess::userCreationResponseArrived,
                     this, &UserStorageGateway::processUserCreationResult);
}


void UserStorageGateway::authenticateUser(domain::models::LoginModel loginModel)
{
    dtos::LoginDto loginDto { .email = loginModel.email(), .password = loginModel.password() };
    
    m_authenticationAccess->authenticateUser(loginDto);
}

void UserStorageGateway::createUser(domain::models::RegisterModel registerModel)
{
    dtos::RegisterDto registerDto { .firstName = registerModel.firstName(),
                .lastName = registerModel.lastName(),
                .email = registerModel.email(),
                .password = registerModel.password() };
    
    m_authenticationAccess->createUser(registerDto);
}


void UserStorageGateway::processAuthenticationResult(bool success, QString token)
{
    emit authenticationResultReady(success, token);
}

void UserStorageGateway::processUserCreationResult(bool success, QString failureReason)
{
    emit userCreationResultReady(success, failureReason);
}

} // namespace adapters::gateways