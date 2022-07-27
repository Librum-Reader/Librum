#pragma once
#include "i_authentication_service.hpp"
#include "i_user_gateway.hpp"


namespace application::services
{

class AuthenticationService : public IAuthenticationService
{
public:
    AuthenticationService(IUserGateway* userGateway);
    
    void authenticateUser(QString email, QString password) override;
    void registerUser(QString email, QString password) override;
    
private:
    IUserGateway* m_userGateway;
};

} // namespace application::services