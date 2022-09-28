#pragma once
#include <QString>
#include <QObject>
#include "login_model.hpp"
#include "register_model.hpp"


namespace application
{
class IAuthenticationService : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IAuthenticationService() noexcept = default;
    virtual void loginUser(const domain::models::LoginModel& loginModel) = 0;
    virtual void registerUser(const domain::models::RegisterModel& registerModel) = 0;
    
signals:
    void loginFinished(bool success);
    void registrationFinished(bool success, const QString& reason);
    void authenticationTokenRegistered(const QString& token);
    void authenticationTokenRemoved();
    
public slots:
    virtual void processAuthenticationResult(const QString& token) = 0;
    virtual void processRegistrationResult(bool success, const QString& reason) = 0;
    virtual void setAuthenticationToken(const QString& token) = 0;
};

} // namespace application