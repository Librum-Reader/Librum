#pragma once
#include <QObject>
#include "i_authentication_service.hpp"
#include "i_user_storage_gateway.hpp"


namespace application::services
{

class AuthenticationService : public IAuthenticationService
{
    Q_OBJECT
    
public:
    AuthenticationService(IUserStorageGateway* userGateway);
    
    void authenticateUser(domain::models::LoginModel loginModel) override;
    void registerUser(domain::models::RegisterModel registerModel) override;
    
private slots:
    void processLoginResult(bool success, QString token);
    void processRegistrationResult(bool success, QString failureReason);
    
private:
    IUserStorageGateway* m_userGateway;
};

} // namespace application::services