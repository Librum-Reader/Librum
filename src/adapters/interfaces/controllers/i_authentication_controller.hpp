#pragma once
#include <QObject>
#include <QString>


namespace adapters
{

class IAuthenticationController : public QObject
{    
public:
    virtual ~IAuthenticationController() noexcept = default;
    
    Q_INVOKABLE virtual void registerUser() = 0;
    Q_INVOKABLE virtual bool authenticateUser(QString email, QString password) = 0;
};

} // namespace adapters