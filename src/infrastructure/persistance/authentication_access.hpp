#pragma once
#include <memory>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QObject>
#include <QDebug>
#include "i_authentication_access.hpp"


namespace infrastructure::persistence
{

class AuthenticationAccess : public adapters::IAuthenticationAccess
{
    Q_OBJECT
    
public:
    AuthenticationAccess();
    
    void authenticateUser(const adapters::dtos::LoginDto& loginDto) override;
    void registerUser(const adapters::dtos::RegisterDto& registerDto) override;
    
private slots:
    void proccessAuthenticationResult();
    void proccessRegistrationResult();
    
private:
    bool checkForErrors(int expectedStatusCode);
    QNetworkRequest createRequest(QUrl url);
    
    const QUrl m_authenticationEndpoint;
    const QUrl m_registrationEndpoint;
    QNetworkAccessManager m_networkAccessManager;
    std::unique_ptr<QNetworkReply> m_reply = nullptr;
};

} // namespace infrastructure::persistence