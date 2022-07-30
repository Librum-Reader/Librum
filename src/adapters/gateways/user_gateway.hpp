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
    
    void authenticateUser(domain::models::LoginModel loginModel) override;
    void createUser(domain::models::RegisterModel registerModel) override;
    
private slots:
    void processAuthenticationToken(bool success, QString token);
    void processUserCreationFailure(QString reason);
    void processUserCreationSuccess();
    
private:
    IAuthenticationAccess* m_authenticationAccess;
};

} // namespace adapters::gateways