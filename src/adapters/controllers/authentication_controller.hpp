#pragma once
#include <QObject>
#include "i_authentication_controller.hpp"
#include "i_authentication_service.hpp"


namespace adapters::controllers
{

class AuthenticationController : public IAuthenticationController
{
    Q_OBJECT
    
public:
    AuthenticationController(application::IAuthenticationService* authenticationService);

    void loginUser(const QString& email, const QString& password) override;
    void registerUser(const QString& firstName, const QString& lastName,
                      const QString& email, QString password,
                      bool keepUpdated) override;
    
public slots:
    void reemitLoginResult(bool success) override;
    void reemitRegistrationResult(bool success, const QString& reason) override;
    
private:
    application::IAuthenticationService* m_authenticationService;
};

} // namespace adapters::controllers