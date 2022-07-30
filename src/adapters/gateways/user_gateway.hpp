#pragma once
#include <QObject>
#include "i_user_gateway.hpp"
#include "i_authentication_access.hpp"


namespace adapters::gateways
{

class UserGateway : public application::IUserGateway
{
    Q_OBJECT
    
public:
    UserGateway(IAuthenticationAccess* authenticationAccess);
    
    void loginUser(domain::models::LoginModel loginModel) override;
    
private slots:
    void processToken(QString token);
    
private:
    IAuthenticationAccess* m_authenticationAccess;
};

} // namespace adapters::gateways