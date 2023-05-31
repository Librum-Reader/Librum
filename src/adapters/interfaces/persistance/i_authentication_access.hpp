#pragma once
#include <QObject>
#include <QString>
#include "login_dto.hpp"
#include "register_dto.hpp"

namespace adapters
{

/**
 * The BookStorageAccess class makes the API calls to the book storage server.
 */
class IAuthenticationAccess : public QObject
{
    Q_OBJECT

public:
    virtual ~IAuthenticationAccess() noexcept = default;
    virtual void authenticateUser(const adapters::dtos::LoginDto& loginDto) = 0;
    virtual void registerUser(
        const adapters::dtos::RegisterDto& registerDto) = 0;

signals:
    void authenticationFinished(const QString& token, int errorCode = -1);
    void registrationFinished(int errorCode = -1);
};

}  // namespace adapters