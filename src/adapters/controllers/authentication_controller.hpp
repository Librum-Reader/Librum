#pragma once
#include <QObject>
#include "adapters_export.hpp"
#include "i_authentication_controller.hpp"
#include "i_authentication_service.hpp"

namespace adapters::controllers
{

class ADAPTERS_EXPORT AuthenticationController
    : public IAuthenticationController
{
    Q_OBJECT

public:
    AuthenticationController(
        application::IAuthenticationService* authenticationService);

    void loginUser(const QString& email, const QString& password,
                   bool rememberUser) override;
    void tryAutomaticLogin() override;
    void loginOffline() override;
    void logoutUser() override;
    void registerUser(const QString& name, const QString& email,
                      QString password, bool keepUpdated) override;
    void checkIfEmailConfirmed(const QString& email) override;

private:
    application::IAuthenticationService* m_authenticationService;
};

}  // namespace adapters::controllers
