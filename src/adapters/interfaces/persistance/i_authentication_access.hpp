#pragma once
#include <QObject>
#include <QString>
#include "login_dto.hpp"


namespace adapters
{

class IAuthenticationAccess : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IAuthenticationAccess() noexcept = default;
    virtual void loginUser(adapters::dtos::LoginDto loginDto) = 0;
    
signals:
    void requestFinished(QString token);
};

} // namespace adapters