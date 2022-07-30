#pragma once
#include <QString>
#include "login_model.hpp"


namespace application
{

class IAuthenticationService
{
public:
    virtual ~IAuthenticationService() noexcept = default;
    
    virtual bool authenticateUser(domain::models::LoginModel loginModel) = 0;
    virtual void registerUser(QString email, QString password) = 0;
};

} // namespace application