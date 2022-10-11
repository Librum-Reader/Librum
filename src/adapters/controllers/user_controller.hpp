#pragma once
#include <QObject>
#include "i_user_controller.hpp"
#include "i_user_service.hpp"


namespace adapters::controllers
{

class UserController : public IUserController
{
    Q_OBJECT
    
public:
    UserController(application::IUserService* userService);
    
    void loadUser() override;
    
    QString getFirstName() override;
    void setFirstName(const QString& newFirstName) override;
    
    QString getLastName() override;
    void setLastName(const QString& newLastName) override;
    
    QString getEmail() override;
    void setEmail(const QString& newEmail) override;
    
private:
    application::IUserService* m_userService;
};

} // namespace adapters::controllers