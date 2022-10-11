#pragma once
#include <QObject>
#include "i_user_storage_gateway.hpp"
#include "i_user_storage_access.hpp"


namespace adapters::gateways
{

class UserStorageGateway : public application::IUserStorageGateway
{
    Q_OBJECT
    
public:
    UserStorageGateway(IUserStorageAccess* userStorageAccess);
    
    void getUser(const QString& authenticationToken) override;
    
private slots:
    void proccessUserData(const QString& firstName, const QString& lastName,
                          const QString& email) override;
    
    void reportFailureGettingUser() override;
    
private:
    IUserStorageAccess* m_userStorageAccess;
};

} // namespace adapters::gateways