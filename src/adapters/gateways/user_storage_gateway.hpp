#pragma once
#include <QObject>
#include "i_user_gateway.hpp"
#include "i_user_storage_access.hpp"


namespace adapters::gateways
{

class UserStorageGateway : public application::IUserGateway
{
    Q_OBJECT
    
public:
    UserStorageGateway(IUserStorageAccess* authenticationAccess);
    
    void authenticateUser(domain::models::LoginModel loginModel) override;
    void createUser(domain::models::RegisterModel registerModel) override;
    
private slots:
    void processAuthenticationResult(bool success, QString token);
    void processUserCreationResult(bool success, QString failureReason);
    
private:
    IUserStorageAccess* m_authenticationAccess;
};

} // namespace adapters::gateways