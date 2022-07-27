#pragma once
#include <QObject>
#include "i_authentication_controller.hpp"


namespace adapters::controllers
{

class AuthenticationController : public IAuthenticationController
{
    Q_OBJECT
    
public:
    AuthenticationController();
    ~AuthenticationController();
    Q_INVOKABLE void registerUser() override;
};

} // namespace adapters::controllers