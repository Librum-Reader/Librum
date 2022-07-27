#pragma once
#include <QObject>


namespace adapters::controllers
{

class IAuthenticationController : public QObject
{    
public:
    virtual ~IAuthenticationController() noexcept = default;
    Q_INVOKABLE virtual void registerUser() = 0;
};

} // namespace adapters::controllers