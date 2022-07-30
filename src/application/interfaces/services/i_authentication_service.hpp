#pragma once
#include <QString>
#include <QObject>
#include "login_model.hpp"


namespace application
{
class IAuthenticationService : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IAuthenticationService() noexcept = default;
    virtual bool authenticateUser(domain::models::LoginModel loginModel) = 0;
    
signals:
    void authenticationSucceeded();
    void authenticationFailed();
};

} // namespace application