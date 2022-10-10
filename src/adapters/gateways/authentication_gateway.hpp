#pragma once
#include "i_authentication_gateway.hpp"
#include "i_authentication_access.hpp"


namespace adapters::gateways
{

class AuthenticationGateway : public application::IAuthenticationGateway
{
    Q_OBJECT
public:
    AuthenticationGateway(IAuthenticationAccess* authenticationAccess);
    
    void authenticateUser(const domain::models::LoginModel& loginModel) override;
    void registerUser(const domain::models::RegisterModel& registerModel) override;
    
public slots:
    void reemitRegistrationResult(bool success, const QString& reason) override;
    void reemitAuthenticationResult(const QString& token) override;
    
private:
    IAuthenticationAccess* m_authenticationAccess;
};

} // namespace adapters::gateways