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
//    AuthenticationAccess();
    
    void authenticateUser(adapters::dtos::LoginDto loginDto) override;
    
private slots:
    void processResult();
    
private:
    QNetworkAccessManager m_networkAccessManager;
    std::unique_ptr<QNetworkReply> m_reply = nullptr;
};

} // namespace infrastructure::persistence