#pragma once
#include "i_authentication_service.hpp"


namespace application::services
{

class AuthenticationService : public IAuthenticationService
{
public:
    AuthenticationService();
    
    void authenticateUser(QString email, QString password) override;
    void registerUser(QString email, QString password) override;
};

} // namespace application::services