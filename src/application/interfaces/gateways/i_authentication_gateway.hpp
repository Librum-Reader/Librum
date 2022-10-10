#pragma once
#include <QObject>
#include <QString>
#include <login_model.hpp>
#include <register_model.hpp>


namespace application
{

class IAuthenticationGateway : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IAuthenticationGateway() noexcept = default;
    virtual void authenticateUser(const domain::models::LoginModel& loginModel) = 0;
    virtual void registerUser(const domain::models::RegisterModel& registerModel) = 0;
    
signals:
    void authenticationFinished(const QString& token);
    void registrationFinished(bool success, const QString& reason);
    
public slots:
    virtual void reemitRegistrationResult(bool success, const QString& reason) = 0;
    virtual void reemitAuthenticationResult(const QString& token) = 0;
};

} // namespace application#pragma once
