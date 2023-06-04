#pragma once
#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QVariantMap>
#include <memory>
#include "i_user_storage_access.hpp"

namespace infrastructure::persistence
{

class UserStorageAccess : public adapters::IUserStorageAccess
{
    Q_OBJECT

public:
    void getUser(const QString& authToken) override;

    void changeFirstName(const QString& authToken,
                         const QString& newFirstName) override;
    void changeLastName(const QString& authToken,
                        const QString& newLastName) override;
    void changeEmail(const QString& authToken,
                     const QString& newEmail) override;
    void changeProfilePicture(const QString& authToken,
                              const QImage& newProfilePicture) override;
    void deleteTag(const QString& authToken, const QString& uuid) override;
    void renameTag(const QString& authToken,
                   const QJsonObject& bookForUpdate) override;

private slots:
    void proccessGetUserResult();

private:
    QNetworkRequest createRequest(const QUrl& url, const QString& authToken);

    QNetworkAccessManager m_networkAccessManager;
};

}  // namespace infrastructure::persistence