#pragma once
#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QSettings>
#include <QVariantMap>
#include <memory>
#include "i_user_storage_access.hpp"

namespace infrastructure::persistence
{

class UserStorageAccess : public adapters::IUserStorageAccess
{
    Q_OBJECT

public:
    UserStorageAccess();

    void getUser(const QString& authToken) override;
    void deleteUser(const QString& authToken) override;
    void forgotPassword(const QString& email) override;
    void getProfilePicture(const QString& authToken) override;
    void changeFirstName(const QString& authToken,
                         const QString& newFirstName) override;
    void changeLastName(const QString& authToken,
                        const QString& newLastName) override;
    void changeEmail(const QString& authToken,
                     const QString& newEmail) override;
    void changePassword(const QString& authToken,
                        const QString& newPassword) override;
    void changeProfilePicture(const QString& authToken,
                              const QString& path) override;
    void deleteProfilePicture(const QString& authToken) override;
    void changeProfilePictureLastUpdated(const QString& authToken,
                                         const QString& newDateTime) override;
    void changeHasProfilePicture(const QString& authToken,
                                 const QString& newValue) override;
    void deleteTag(const QString& authToken, const QString& uuid) override;
    void renameTag(const QString& authToken,
                   const QJsonObject& bookForUpdate) override;

private:
    void addImagePartToMultiPart(QHttpMultiPart* multiPart,
                                 const QString& imagePath);
    QNetworkRequest createRequest(const QUrl& url, const QString& authToken);

    QNetworkAccessManager m_networkAccessManager;
    QString domain;
};

}  // namespace infrastructure::persistence
