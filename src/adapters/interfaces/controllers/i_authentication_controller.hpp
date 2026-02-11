#pragma once
#include <QObject>
#include <QString>
#include "adapters_export.hpp"

namespace adapters
{

/**
 * The AuthenticationController class is exposed to the UI code and thus is the
 * "entry point" to the application's backend for authentication operations. It
 * acts as a layer of abstraction which maps the user data to a format usable
 * for the application.
 */
class ADAPTERS_EXPORT IAuthenticationController : public QObject
{
    Q_OBJECT

public:
    virtual ~IAuthenticationController() noexcept = default;

    Q_INVOKABLE virtual void loginUser(const QString& email,
                                       const QString& password,
                                       bool rememberUser) = 0;
    Q_INVOKABLE virtual void tryAutomaticLogin() = 0;
    Q_INVOKABLE virtual void loginOffline() = 0;
    Q_INVOKABLE virtual void logoutUser() = 0;
    Q_INVOKABLE virtual void registerUser(const QString& name,
                                          const QString& email,
                                          QString password,
                                          bool keepUpdated) = 0;
    Q_INVOKABLE virtual void checkIfEmailConfirmed(const QString& email) = 0;

signals:
    void loginFinished(int errorCode, const QString& message);
    void registrationFinished(int errorCode, const QString& message);
    void emailConfirmationCheckFinished(bool confirmed);
};

}  // namespace adapters
