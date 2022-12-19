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
    void renameTag(const QString& authToken, const QString& uuid,
                   const QJsonObject& bookForUpdate) override;

private slots:
    void proccessGetUserResult();
    void linkRequestToErrorHandling(QNetworkReply* reply, int statusCode);

private:
    QNetworkRequest createRequest(const QUrl& url, const QString& authToken);
    bool checkForErrors(int expectedStatusCode, QNetworkReply* reply);

    QNetworkAccessManager m_networkAccessManager;
    std::unique_ptr<QNetworkReply> m_getUserReply = nullptr;
    std::unique_ptr<QNetworkReply> m_firstNameUpdateReply = nullptr;
    std::unique_ptr<QNetworkReply> m_lastNameUpdateReply = nullptr;
    std::unique_ptr<QNetworkReply> m_emailUpdateReply = nullptr;
    std::unique_ptr<QNetworkReply> m_profilePictureUpdateReply = nullptr;
    std::unique_ptr<QNetworkReply> m_tagDeletionReply = nullptr;
    std::unique_ptr<QNetworkReply> m_tagRenameReply = nullptr;
};

}  // namespace infrastructure::persistence