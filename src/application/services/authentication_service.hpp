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

private:
    void clearTemporaryUserData();

    IAuthenticationGateway* m_authenticationGateway;
    QString m_token;

    // To be able to send the user email with the 'loggedIn' signal
    QString m_tempEmail;
};

}  // namespace application::services