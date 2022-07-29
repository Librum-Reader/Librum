#pragma once
#include "i_user_gateway.hpp"
#include "i_authentication_access.hpp"


namespace adapters::gateways
{

class UserGateway : public application::IUserGateway
{
public:
    UserGateway(IAuthenticationAccess* authenticationAccess);
    
    void loginUser(QString email, QString password) override;
    
private:
    IAuthenticationAccess* m_authenticationAccess;
};

} // namespace adapters::gateways