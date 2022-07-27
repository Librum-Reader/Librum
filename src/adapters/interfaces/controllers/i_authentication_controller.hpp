#pragma once
#include <QObject>


namespace adapters
{

class IAuthenticationController : public QObject
{    
public:
    virtual ~IAuthenticationController() noexcept = default;
    
    Q_INVOKABLE virtual void registerUser() = 0;
    Q_INVOKABLE virtual void authenticateUser() = 0;
};

} // namespace adapters