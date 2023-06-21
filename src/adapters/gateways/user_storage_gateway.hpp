#pragma once
#include <QObject>
#include "i_user_storage_access.hpp"
#include "i_user_storage_gateway.hpp"

namespace adapters::gateways
{

class UserStorageGateway : public application::IUserStorageGateway
{
    Q_OBJECT

public:
    UserStorageGateway(IUserStorageAccess* userStorageAccess);

    void getUser(const QString& authToken) override;
    void getProfilePicture(const QString& authToken) override;
    void changeFirstName(const QString& authToken,
                         const QString& newFirstName) override;
    void changeLastName(const QString& authToken,
                        const QString& newLastName) override;
    void changeEmail(const QString& authToken,
                     const QString& newEmail) override;
    void changeProfilePicture(const QString& authToken,
                              const QString& path) override;
    void deleteProfilePicture(const QString& authToken) override;
    void changeProfilePictureLastUpdated(const QString& authToken,
                                         const QDateTime& newDateTime) override;
    void changeHasProfilePicture(const QString& authToken,
                                 bool newValue) override;
    void deleteTag(const QString& authToken, const QUuid& uuid) override;
    void renameTag(const QString& authToken, const QUuid& uuid,
                   const QString& newName) override;

private slots:
    void proccessUserData(const QString& firstName, const QString& lastName,
                          const QString& email, long usedBookStorage,
                          long bookStorageLimit,
                          const QDateTime& profilePictureLastUpdated,
                          bool hasProfilePicture, const QJsonArray& tags);

    void reportFailureGettingUser();

private:
    void renameJsonObjectKey(QJsonObject& jsonObject, const QString& oldKeyName,
                             const QString& newKeyName);

    IUserStorageAccess* m_userStorageAccess;
    QString m_dateTimeFormat = "hh:mm:ss - dd.MM.yyyy";
};

}  // namespace adapters::gateways
