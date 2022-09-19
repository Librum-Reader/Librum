#include "user_storage_gateway.hpp"
#include "i_user_storage_access.hpp"
#include "login_dto.hpp"
#include "register_dto.hpp"


namespace adapters::gateways
{

UserStorageGateway::UserStorageGateway(IUserStorageAccess* authenticationAccess)
    : m_authenticationAccess(authenticationAccess)
{
    QObject::connect(m_authenticationAccess, &IUserStorageAccess::authenticationFinished,
                     this, &UserStorageGateway::reemitAuthenticationResult);
    
    QObject::connect(m_authenticationAccess, &IUserStorageAccess::registrationFinished,
                     this, &UserStorageGateway::reemitRegistrationResult);
}


void UserStorageGateway::authenticateUser(const domain::models::LoginModel& loginModel)
{
    dtos::LoginDto loginDto
    {
        .email = loginModel.email(),
        .password = loginModel.password()
    };
    
    m_authenticationAccess->authenticateUser(loginDto);
}

void UserStorageGateway::registerUser(const domain::models::RegisterModel& registerModel)
{
    dtos::RegisterDto registerDto
    {
        .firstName = registerModel.firstName(),
        .lastName = registerModel.lastName(),
        .email = registerModel.email(),
        .password = registerModel.password()
    };
    
    m_authenticationAccess->regsiterUser(registerDto);
}

void UserStorageGateway::reemitRegistrationResult(bool success, const QString& reason)
{
    emit registrationFinished(success, reason);
}

void UserStorageGateway::reemitAuthenticationResult(const QString& token)
{
    emit authenticationFinished(token);
}




} // namespace adapters::gateways