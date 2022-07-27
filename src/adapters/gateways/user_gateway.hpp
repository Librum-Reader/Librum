#pragma once
#include "i_user_gateway.hpp"


namespace adapters::gateways
{

class UserGateway : public application::IUserGateway
{
public:
    UserGateway();
    
    void userExists(QString email, QString password) override;
    void createUser(QString email, QString password) override;
};

} // namespace adapters::gateways