#pragma once
#include <QObject>
#include "application_export.hpp"
#include "i_authentication_gateway.hpp"
#include "i_authentication_service.hpp"

namespace application::services
{

class APPLICATION_EXPORT AuthenticationService : public IAuthenticationService
{
    Q_OBJECT

public:
    AuthenticationService(IAuthenticationGateway* authenticationGateway);

    void loginUser(
        const domain::value_objects::LoginModel& loginModel) override;
    void tryAutomaticLogin() override;
    void loginOffline() override;
    void registerUser(
        const domain::value_objects::RegisterModel& registerModel) override;
    void checkIfEmailConfirmed(const QString& email) override;

public slots:
    void logoutUser() override;
    void processAuthenticationResult(const QString& token,
                                     ErrorCode errorCode) override;
    void processRegistrationResult(ErrorCode errorCode) override;

private:
    void clearTemporaryUserData();

    IAuthenticationGateway* m_authenticationGateway;
    bool m_rememberUser = false;
    // To be able to send the user's email with the 'loggedIn' signal.
    QString m_tempEmail;
};

}  // namespace application::services