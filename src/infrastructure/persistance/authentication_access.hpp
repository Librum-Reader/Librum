#pragma once
#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <memory>
#include "i_authentication_access.hpp"
#include "server_reply_status.hpp"

namespace infrastructure::persistence
{

class AuthenticationAccess : public adapters::IAuthenticationAccess
{
    Q_OBJECT

public:
    void authenticateUser(const adapters::dtos::LoginDto& loginDto) override;
    void registerUser(const adapters::dtos::RegisterDto& registerDto) override;

private:
    ServerReplyStatus validateNetworkReply(int expectedStatusCode,
                                           QNetworkReply*, const QString& name);
    QString getErrorMessageFromReply(const QByteArray& reply);
    int getErrorCodeFromReply(const QByteArray& reply);
    QNetworkRequest createRequest(QUrl url);

    QNetworkAccessManager m_networkAccessManager;
};

}  // namespace infrastructure::persistence