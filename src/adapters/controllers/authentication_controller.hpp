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
        
    Q_INVOKABLE bool loginUser(QString email, QString password) override;
    Q_INVOKABLE bool registerUser(QString firstName, QString lastName, QString email,
                                  QString password, bool keepUpdated) override;

private slots:
    void loginSuccess();
    void loginFailure();
    void registrationSuccess();
    void registrationFailure(QString reason);
    
private:
    application::IAuthenticationService* m_authenticationService;
};

} // namespace adapters::controllers