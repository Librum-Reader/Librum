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
    AuthenticationController(application::services::IAuthenticationService* authenticationService);
    ~AuthenticationController();
    
    Q_INVOKABLE void registerUser() override;
    Q_INVOKABLE void authenticateUser() override;
    
private:
    application::services::IAuthenticationService* m_authenticationService;
};

} // namespace adapters::controllers