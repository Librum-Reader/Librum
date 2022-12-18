#pragma once
#include <QObject>
#include "i_authentication_gateway.hpp"
#include "i_authentication_service.hpp"

namespace application::services
{

class AuthenticationService : public IAuthenticationService
{
    Q_OBJECT

public:
    AuthenticationService(IAuthenticationGateway* authenticationGateway);

    void loginUser(
        const domain::value_objects::LoginModel& loginModel) override;
    void logoutUser() override;
    void registerUser(
        const domain::value_objects::RegisterModel& registerModel) override;

public slots:
    void processAuthenticationResult(const QString& token) override;
    void processRegistrationResult(bool success,
                                   const QString& reason) override;
    void setAuthenticationToken(const QString& token,
                                const QString& email) override;

private:
    IAuthenticationGateway* m_authenticationGateway;
    QString m_currentEmail;
    QString m_token;
};

}  // namespace application::services