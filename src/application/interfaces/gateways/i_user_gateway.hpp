#pragma once
#include <QString>
#include "../../../domain/models/login_model.hpp"


namespace application
{

class IUserGateway
{
public:
    virtual ~IUserGateway() noexcept = default;
    
    virtual QString loginUser(domain::models::LoginModel loginModel) = 0;
};

} // namespace application