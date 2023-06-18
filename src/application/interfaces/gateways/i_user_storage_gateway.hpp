#pragma once
#include <QObject>
#include <QString>
#include <QUuid>
#include "user.hpp"

namespace application
{

/**
 * The UserStorageGateway class acts as a layer of abstraction before the
 * UserStorageAccess class. It maps the data provided by the application to
 * the data type required for user storage API requests.
 */
class IUserStorageGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~IUserStorageGateway() noexcept = default;

    virtual void getUser(const QString& authToken) = 0;
    virtual void getProfilePicture(const QString& authToken) = 0;
    virtual void changeFirstName(const QString& authToken,
                                 const QString& newFirstName) = 0;
    virtual void changeLastName(const QString& authToken,
                                const QString& newLastName) = 0;
    virtual void changeEmail(const QString& authToken,
                             const QString& newEmail) = 0;
    virtual void changeProfilePicture(const QString& authToken,
                                      const QString& path) = 0;
    virtual void deleteProfilePicture(const QString& authToken) = 0;
<<<<<<< HEAD
    virtual void changeProfilePictureLastUpdated(
        const QString& authToken, const QDateTime& newDateTime) = 0;
    virtual void changeHasProfilePicture(const QString& authToken,
                                         bool newValue) = 0;
=======
>>>>>>> upstream/dev/develop
    virtual void deleteTag(const QString& authToken, const QUuid& uuid) = 0;
    virtual void renameTag(const QString& authToken, const QUuid& uuid,
                           const QString& newName) = 0;

signals:
    void finishedGettingUser(const domain::entities::User& user, bool success);
    void profilePictureReady(QByteArray& data);
};

}  // namespace application
