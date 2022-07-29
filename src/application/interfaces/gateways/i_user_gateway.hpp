#pragma once
#include <QString>


namespace application
{

class IUserGateway
{
public:
    virtual ~IUserGateway() noexcept = default;
    
    virtual void loginUser(QString email, QString password) = 0;
};

} // namespace application