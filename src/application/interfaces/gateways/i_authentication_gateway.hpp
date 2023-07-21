#pragma once
#include <QObject>
#include <QString>
#include <login_model.hpp>
#include <register_model.hpp>
#include "error_code.hpp"
#include "application_export.hpp"

namespace application
{

/**
 * The AuthenticationGateway class acts as a layer of abstraction before the
 * AuthenticationAccess class. It maps the data provided by the application to
 * the data type required for authentication API requests.
 */
class APPLICATION_LIBRARY IAuthenticationGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~IAuthenticationGateway() noexcept = default;
    virtual void authenticateUser(
        const domain::value_objects::LoginModel& loginModel) = 0;
    virtual void registerUser(
        const domain::value_objects::RegisterModel& registerModel) = 0;
    virtual void checkIfEmailConfirmed(const QString& email) = 0;

signals:
    void authenticationFinished(const QString& token, ErrorCode errorCode);
    void registrationFinished(ErrorCode errorCode);
    void emailConfirmationCheckFinished(bool confirmed);
};

}  // namespace application
