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
    
    Q_INVOKABLE virtual void loginUser(const QString& email, const QString& password) = 0;
    Q_INVOKABLE virtual void registerUser(const QString& firstName, const QString& lastName,
                                          const QString& email, QString password, 
                                          bool keepUpdated) = 0;
    
public slots:
    virtual void reemitLoginResult(bool success) = 0;
    virtual void reemitRegistrationResult(bool success, const QString& reason) = 0;
    
signals:
    void loginFinished(bool success);
    void registrationFinished(bool success, const QString& reason);
};

} // namespace adapters