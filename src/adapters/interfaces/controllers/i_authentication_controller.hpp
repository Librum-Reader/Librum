#pragma once
#include <QObject>
#include <QString>

namespace adapters
{

/**
 * The AuthenticationController class is exposed to the UI code and thus is the
 * "entry point" to the application's backend for authentication operations. It
 * acts as a layer of abstraction which maps the user data to a format usable
 * for the application.
 */
class IAuthenticationController : public QObject
{
    Q_OBJECT

public:
    virtual ~IAuthenticationController() noexcept = default;

    Q_INVOKABLE virtual void loginUser(const QString& email,
                                       const QString& password,
                                       bool rememberUser) = 0;
    Q_INVOKABLE virtual void tryAutomaticLogin() = 0;
    Q_INVOKABLE virtual void logoutUser() = 0;
    Q_INVOKABLE virtual void registerUser(const QString& firstName,
                                          const QString& lastName,
                                          const QString& email,
                                          QString password,
                                          bool keepUpdated) = 0;

signals:
    void loginFinished(bool success, const QString& message);
    void registrationFinished(bool success, const QString& message);
};

}  // namespace adapters