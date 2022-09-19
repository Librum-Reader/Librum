#pragma once
#include <QObject>
#include <QString>
#include "login_dto.hpp"
#include "register_dto.hpp"


namespace adapters
{

class IUserStorageAccess : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IUserStorageAccess() noexcept = default;
    virtual void authenticateUser(const adapters::dtos::LoginDto& loginDto) = 0;
    virtual void regsiterUser(const adapters::dtos::RegisterDto& registerDto) = 0;
    
signals:
    void authenticationFinished(const QString& token);
    void registrationFinished(bool success, const QString& reason);
};

} // namespace adapters