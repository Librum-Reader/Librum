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
#include "i_user_storage_access.hpp"


namespace infrastructure::persistence
{

class UserStorageAccess : public adapters::IUserStorageAccess
{
    Q_OBJECT
    
public:
    UserStorageAccess();
    
    void authenticateUser(const adapters::dtos::LoginDto& loginDto) override;
    void regsiterUser(const adapters::dtos::RegisterDto& registerDto) override;
    
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