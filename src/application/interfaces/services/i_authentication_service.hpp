#pragma once
#include <QObject>
#include <QString>
#include "application_export.hpp"
#include "error_code.hpp"
#include "login_model.hpp"
#include "register_model.hpp"

namespace application
{

/**
 *  The AuthenticationService handles the authentication.
 */
class APPLICATION_EXPORT IAuthenticationService : public QObject
{
    Q_OBJECT

public:
    virtual ~IAuthenticationService() noexcept = default;
    virtual void loginUser(
        const domain::value_objects::LoginModel& loginModel) = 0;
    virtual void tryAutomaticLogin() = 0;
    virtual void loginOffline() = 0;
    virtual void registerUser(
        const domain::value_objects::RegisterModel& registerModel) = 0;
    virtual void checkIfEmailConfirmed(const QString& email) = 0;

public slots:
    virtual void logoutUser() = 0;

signals:
    void loginFinished(ErrorCode errorCode, const QString& message = "");
    void registrationFinished(ErrorCode errorCode, const QString& message);
    void loggedIn(const QString& token, const QString& email);
    void loggedOut();
    void emailConfirmationCheckFinished(bool confirmed);

public slots:
    virtual void processAuthenticationResult(const QString& token,
                                             ErrorCode errorCode) = 0;
    virtual void processRegistrationResult(ErrorCode errorCode) = 0;
};

}  // namespace application