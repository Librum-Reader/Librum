#pragma once
#include <QObject>
#include <QString>
#include "login_dto.hpp"


namespace adapters
{

class IAuthenticationAccess
{
public:
    virtual ~IAuthenticationAccess() noexcept = default;
    
    virtual QString loginUser(adapters::dtos::LoginDto loginDto) = 0;
};

} // namespace adapters