#pragma once
#include <QObject>
#include <QString>
#include "login_dto.hpp"
#include "register_dto.hpp"


namespace adapters
{

class IAuthenticationAccess : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IAuthenticationAccess() noexcept = default;
    virtual void authenticateUser(adapters::dtos::LoginDto loginDto) = 0;
    virtual void createUser(adapters::dtos::RegisterDto registerDto) = 0;
    
signals:
    void authenticationResponseReceived(bool success, QString token);
    void userCreationFailed(QString reason);
};

} // namespace adapters