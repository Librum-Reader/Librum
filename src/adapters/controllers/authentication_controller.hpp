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
        
    Q_INVOKABLE void registerUser() override;
    Q_INVOKABLE bool authenticateUser(QString email, QString password) override;
    
private:
    application::IAuthenticationService* m_authenticationService;
};

} // namespace adapters::controllers