#pragma once
#include <QImage>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include "adapters_export.hpp"

namespace adapters
{

/**
 * The UserStorageAccess class makes the API calls to the user storage server.
 */
class ADAPTERS_EXPORT IUserStorageAccess : public QObject
{
    Q_OBJECT

public:
    virtual ~IUserStorageAccess() noexcept = default;

    virtual void getUser(const QString& authToken) = 0;
    virtual void deleteUser(const QString& authToken) = 0;
    virtual void forgotPassword(const QString& email) = 0;
    virtual void getProfilePicture(const QString& authToken) = 0;
    virtual void changeFirstName(const QString& authToken,
                                 const QString& newFirstName) = 0;
    virtual void changeLastName(const QString& authToken,
                                const QString& newLastName) = 0;
    virtual void changeEmail(const QString& authToken,
                             const QString& newEmail) = 0;
    virtual void changePassword(const QString& authToken,
                                const QString& newPassword) = 0;
    virtual void changeProfilePicture(const QString& authToken,
                                      const QString& path) = 0;
    virtual void deleteProfilePicture(const QString& authToken) = 0;
    virtual void changeProfilePictureLastUpdated(
        const QString& authToken, const QString& newDateTime) = 0;
    virtual void changeHasProfilePicture(const QString& authToken,
                                         const QString& newValue) = 0;
    virtual void deleteTag(const QString& authToken, const QString& uuid) = 0;
    virtual void renameTag(const QString& authToken,
                           const QJsonObject& bookForUpdate) = 0;

signals:
    void authTokenExpired();
    void userReady(const QByteArray& data);
    void gettingUserFailed();
    void profilePictureReady(QByteArray& data);
    void passwordChangeFinished(bool success, const QString& reason);
};

}  // namespace adapters
