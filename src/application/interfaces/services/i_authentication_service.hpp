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
    virtual void logoutUser() = 0;
    virtual void registerUser(
        const domain::value_objects::RegisterModel& registerModel) = 0;

signals:
    void loginFinished(bool success);
    void registrationFinished(bool success, const QString& reason);
    void loggedIn(const QString& token, const QString& email);
    void loggedOut();

public slots:
    virtual void processAuthenticationResult(const QString& token) = 0;
    virtual void processRegistrationResult(bool success,
                                           const QString& reason) = 0;
    virtual void setAuthenticationToken(const QString& token,
                                        const QString& email) = 0;
};

}  // namespace application