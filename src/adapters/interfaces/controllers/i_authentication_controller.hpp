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
    
    Q_INVOKABLE virtual bool loginUser(QString email, QString password) = 0;
    Q_INVOKABLE virtual bool registerUser(QString firstName, QString lastName, QString email,
                                          QString password, bool keepUpdated) = 0;
    
signals:
    void loginSucceeded();
    void loginFailed();
};

} // namespace adapters