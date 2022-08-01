#pragma once
#include <QObject>
#include <QString>
#include "login_model.hpp"
#include "register_model.hpp"


namespace application
{

class IUserStorageGateway : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IUserStorageGateway() noexcept = default;
    virtual void authenticateUser(domain::models::LoginModel loginModel) = 0;
    virtual void createUser(domain::models::RegisterModel registerModel) = 0;
    
signals:
    void authenticationResultReady(bool success, QString token);
    void userCreationResultReady(bool success, QString failureReason);
};

} // namespace application