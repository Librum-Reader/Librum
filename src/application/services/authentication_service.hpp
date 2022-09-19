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
    AuthenticationService(IUserStorageGateway* userStorageGateway);
    
    void loginUser(const domain::models::LoginModel& loginModel) override;
    void registerUser(const domain::models::RegisterModel& registerModel) override;
    
public slots:
    void processAuthenticationResult(const QString& token) override;
    void processRegistrationResult(bool success, const QString& reason) override;
    void setAuthenticationToken(const QString& token) override;
    
private:
    IUserStorageGateway* m_userStorageGateway;
    QString m_token;
};

} // namespace application::services