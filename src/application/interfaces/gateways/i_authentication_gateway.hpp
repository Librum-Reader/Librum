#pragma once
#include <QObject>
#include <QString>
#include <login_model.hpp>
#include <register_model.hpp>
#include "api_error_codes.hpp"

namespace application
{

/**
 * The AuthenticationGateway class acts as a layer of abstraction before the
 * AuthenticationAccess class. It maps the data provided by the application to
 * the data type required for authentication API requests.
 */
class IAuthenticationGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~IAuthenticationGateway() noexcept = default;
    virtual void authenticateUser(
        const domain::value_objects::LoginModel& loginModel) = 0;
    virtual void registerUser(
        const domain::value_objects::RegisterModel& registerModel) = 0;

signals:
    void authenticationFinished(const QString& token, ApiErrorCodes errorCode);
    void registrationFinished(ApiErrorCodes errorCode);
};

}  // namespace application
