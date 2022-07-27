#pragma once
#include <QString>

namespace application::services
{

class IAuthenticationService
{
public:
    virtual ~IAuthenticationService() noexcept = default;
    
    virtual void authenticateUser(QString email, QString password) = 0;
    virtual void registerUser(QString email, QString password) = 0;
};

} // namespace application::services