#pragma once
#include <QObject>
#include <QString>
#include "../../../domain/models/login_model.hpp"


namespace application
{

class IUserGateway : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IUserGateway() noexcept = default;
    virtual void loginUser(domain::models::LoginModel loginModel) = 0;
    
signals:
    void resultReady(QString token);
};

} // namespace application