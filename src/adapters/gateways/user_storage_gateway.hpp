#pragma once
#include <QObject>
#include "i_user_storage_gateway.hpp"
#include "i_user_storage_access.hpp"


namespace adapters::gateways
{

class UserStorageGateway : public application::IUserStorageGateway
{
    Q_OBJECT
    
public:
    UserStorageGateway(IUserStorageAccess* authenticationAccess);
    
    void authenticateUser(const domain::models::LoginModel& loginModel) override;
    void registerUser(const domain::models::RegisterModel& registerModel) override;
    
public slots:
    void reemitRegistrationResult(bool success, const QString& reason) override;
    void reemitAuthenticationResult(const QString& token) override;
    
private:
    IUserStorageAccess* m_authenticationAccess;
};

} // namespace adapters::gateways