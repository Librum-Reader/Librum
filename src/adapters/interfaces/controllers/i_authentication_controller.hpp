#pragma once
#include <QObject>
#include <QString>


namespace adapters
{

class IAuthenticationController : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IAuthenticationController() noexcept = default;
    
    Q_INVOKABLE virtual bool authenticateUser(QString email, QString password) = 0;
    
signals:
    void authenticationSucceeded();
    void authenticationFailed();
};

} // namespace adapters