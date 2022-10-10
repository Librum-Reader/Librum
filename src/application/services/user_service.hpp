#pragma once
#include <QObject>
#include "i_user_service.hpp"
#include "i_user_storage_gateway.hpp"


namespace application::services
{

class UserService : public IUserService
{
    Q_OBJECT
    
public:
    UserService(IUserStorageGateway* userStorageGateway);
    
    QString getFirstName() override;
    void setFirstName(const QString& newFirstName) override;
    
    QString getLastName() override;
    void setLastName(const QString& newLastName) override;
    
    QString getEmail() override;
    void setEmail(const QString& newEmail) override;
    
public slots:
    void setAuthenticationToken(const QString& token) override;
    void clearAuthenticationToken() override;

private:
    IUserStorageGateway* m_userStorageGateway;
    QString m_firstName;
    QString m_lastName;
    QString m_email;
    QString m_authenticationToken;    
};

} // namespace application::services