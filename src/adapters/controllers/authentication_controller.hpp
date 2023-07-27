#pragma once
#include <QObject>
#include "i_authentication_controller.hpp"
#include "i_authentication_service.hpp"
#include "adapters_export.hpp"

namespace adapters::controllers
{

class ADAPTERS_LIBRARY AuthenticationController : public IAuthenticationController
{
    Q_OBJECT

public:
    AuthenticationController(
        application::IAuthenticationService* authenticationService);

    void loginUser(const QString& email, const QString& password,
                   bool rememberUser) override;
    void tryAutomaticLogin() override;
    void logoutUser() override;
    void registerUser(const QString& firstName, const QString& lastName,
                      const QString& email, QString password,
                      bool keepUpdated) override;
    void checkIfEmailConfirmed(const QString& email) override;

private:
    application::IAuthenticationService* m_authenticationService;
};

}  // namespace adapters::controllers
