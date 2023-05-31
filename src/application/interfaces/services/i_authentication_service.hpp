#pragma once
#include <QObject>
#include <QString>
#include "login_model.hpp"
#include "register_model.hpp"

namespace application
{

/**
 *  The AuthenticationService handles the authentication.
 */
class IAuthenticationService : public QObject
{
    Q_OBJECT

public:
    virtual ~IAuthenticationService() noexcept = default;
    virtual void loginUser(
        const domain::value_objects::LoginModel& loginModel) = 0;
    virtual void tryAutomaticLogin() = 0;
    virtual void logoutUser() = 0;
    virtual void registerUser(
        const domain::value_objects::RegisterModel& registerModel) = 0;

signals:
    void loginFinished(bool success, const QString& message = "");
    void registrationFinished(bool success, const QString& message);
    void loggedIn(const QString& token, const QString& email);
    void loggedOut();

public slots:
    virtual void processAuthenticationResult(const QString& token,
                                             int errorCode = -1) = 0;
    virtual void processRegistrationResult(int errorCode) = 0;
};

}  // namespace application