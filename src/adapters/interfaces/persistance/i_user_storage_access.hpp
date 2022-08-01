#pragma once
#include <QObject>
#include <QString>
#include "login_dto.hpp"
#include "register_dto.hpp"


namespace adapters
{

class IUserStorageAccess : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IUserStorageAccess() noexcept = default;
    virtual void authenticateUser(adapters::dtos::LoginDto loginDto) = 0;
    virtual void createUser(adapters::dtos::RegisterDto registerDto) = 0;
    
signals:
    void authenticationResponseArrived(bool success, QString token);
    void userCreationResponseArrived(bool success, QString failureReason);
};

} // namespace adapters