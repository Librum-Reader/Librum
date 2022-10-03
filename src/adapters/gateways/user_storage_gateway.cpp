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
        .email = loginModel.getEmail(),
        .password = loginModel.getPassword()
    };
    
    m_authenticationAccess->authenticateUser(loginDto);
}

void UserStorageGateway::registerUser(const domain::models::RegisterModel& registerModel)
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

void UserStorageGateway::reemitRegistrationResult(bool success, const QString& reason)
{
    emit registrationFinished(success, reason);
}

void UserStorageGateway::reemitAuthenticationResult(const QString& token)
{
    emit authenticationFinished(token);
}




} // namespace adapters::gateways