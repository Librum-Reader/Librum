#pragma once
#include <QObject>
#include <QString>
#include "login_model.hpp"


namespace application
{

class IUserGateway : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IUserGateway() noexcept = default;
    virtual void authenticateUser(domain::models::LoginModel loginModel) = 0;
    
signals:
    void resultReady(QString token);
};

} // namespace application