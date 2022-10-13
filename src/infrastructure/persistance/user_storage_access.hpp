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
    
    void getUser(const QString& authToken) override;
    
    void changeFirstName(const QString& authToken,
                         const QString& newFirstName) override;
    void changeLastName(const QString& authToken,
                        const QString& newLastName) override;
    void changeEmail(const QString& authToken,
                     const QString& newEmail) override;
    void changeProfilePicture(const QString& authToken, 
                              const QImage& newProfilePicture) override;
    
private slots:
    void proccessGetUserResult();
    
private:
    QNetworkRequest createRequest(const QUrl& url, const QString& authToken);
    bool checkForErrors(int expectedStatusCode);
    QVariantMap parseJsonToMap(QByteArray jsonBytes);
    
    const QUrl m_getUserEndpoint;
    const QUrl m_patchUserEndpoint;
    QNetworkAccessManager m_networkAccessManager;
    std::unique_ptr<QNetworkReply> m_reply = nullptr;
};

} // namespace infrastructure::persistence