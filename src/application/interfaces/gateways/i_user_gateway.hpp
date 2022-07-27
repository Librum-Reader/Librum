#pragma once
#include <QString>


namespace application
{

class IUserGateway
{
public:
    virtual ~IUserGateway() noexcept = default;
    
    virtual void userExists(QString email, QString password) = 0;
    virtual void createUser(QString email, QString password) = 0;
};

} // namespace application