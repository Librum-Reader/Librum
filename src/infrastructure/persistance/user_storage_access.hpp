#pragma once
#include <memory>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
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
    
    void getUser(const QString& authenticationToken) override;
    
private slots:
    void proccessGetUserResult() override;
    
private:
    QNetworkRequest createRequest(const QUrl& url, const QString& authToken);
    bool checkForErrors(int expectedStatusCode);
    QVariantMap parseJsonToMap(QByteArray jsonBytes);
    
    const QUrl m_getUserEndpoint;
    QNetworkAccessManager m_networkAccessManager;
    std::unique_ptr<QNetworkReply> m_reply = nullptr;
};

} // namespace infrastructure::persistence