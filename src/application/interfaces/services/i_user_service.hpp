#pragma once
#include <QDir>
#include <QImage>
#include <QObject>
#include <QString>
#include <QUuid>
#include <vector>
#include "application_export.hpp"
#include "tag.hpp"

namespace application
{

/**
 *  The LibraryService handles all the operations on the user.
 */
class APPLICATION_EXPORT IUserService : public QObject
{
    Q_OBJECT

public:
    virtual ~IUserService() noexcept = default;

    virtual void loadUser(bool rememberUser) = 0;
    virtual void deleteUser() = 0;
    virtual void downloadUser() = 0;

    virtual QString getFirstName() const = 0;
    virtual void setFirstName(const QString& newFirstName) = 0;

    virtual QString getLastName() const = 0;
    virtual void setLastName(const QString& newLastName) = 0;

    virtual QString getEmail() const = 0;
    virtual void setEmail(const QString& newEmail) = 0;

    virtual qint64 getUsedBookStorage() const = 0;
    virtual qint64 getBookStorageLimit() const = 0;

    virtual QString getProfilePicturePath() const = 0;
    virtual void setProfilePicturePath(const QString& path) = 0;

    virtual void deleteProfilePicture() = 0;
    virtual void changePassword(const QString& newPassword) = 0;
    virtual void forgotPassword(const QString& email) = 0;

    virtual const std::vector<domain::entities::Tag>& getTags() const = 0;
    virtual QUuid addTag(const domain::entities::Tag& tag) = 0;
    virtual bool deleteTag(const QUuid& uuid) = 0;
    virtual bool renameTag(const QUuid& uuid, const QString& newName) = 0;

signals:
    void finishedLoadingUser(bool success);
    void profilePictureChanged();
    void tagInsertionStarted(int index);
    void tagInsertionEnded();
    void tagDeletionStarted(int index);
    void tagDeletionEnded();
    void tagsChanged(int index);
    void bookStorageDataUpdated(qint64 usedStorage, qint64 bookStorageLimit);
    void passwordChangeFinished(bool success, const QString& reason);

public slots:
    virtual void setupUserData(const QString& token, const QString& email) = 0;
    virtual void clearUserData() = 0;
};

}  // namespace application
