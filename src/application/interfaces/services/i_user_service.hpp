#pragma once
#include <QDir>
#include <QImage>
#include <QObject>
#include <QString>
#include <QUuid>
#include <vector>
#include "tag.hpp"

namespace application
{

/**
 *  The BookService handles all the operations on the user.
 */
class IUserService : public QObject
{
    Q_OBJECT

public:
    virtual ~IUserService() noexcept = default;

    virtual void loadUser(bool rememberUser) = 0;

    virtual QString getFirstName() const = 0;
    virtual void setFirstName(const QString& newFirstName) = 0;

    virtual QString getLastName() const = 0;
    virtual void setLastName(const QString& newLastName) = 0;

    virtual QString getEmail() const = 0;
    virtual void setEmail(const QString& newEmail) = 0;

    virtual long getUsedBookStorage() const = 0;
    virtual long getBookStorageLimit() const = 0;

    virtual QImage getProfilePicture() const = 0;
    virtual void setProfilePicture(const QString& path,
                                   const QImage& image) = 0;
    virtual void saveProfilePictureToFile(QByteArray& data) = 0;

    virtual const std::vector<domain::entities::Tag>& getTags() const = 0;
    virtual QUuid addTag(const domain::entities::Tag& tag) = 0;
    virtual bool deleteTag(const QUuid& uuid) = 0;
    virtual bool renameTag(const QUuid& uuid, const QString& newName) = 0;

    virtual QDir getUserProfileDir() const = 0;

signals:
    void finishedLoadingUser(bool success);
    void tagInsertionStarted(int index);
    void tagInsertionEnded();
    void tagDeletionStarted(int index);
    void tagDeletionEnded();
    void tagsChanged(int index);
    void bookStorageDataUpdated(long usedStorage, long bookStorageLimit);

public slots:
    virtual void setupUserData(const QString& token, const QString& email) = 0;
    virtual void clearUserData() = 0;
};

}  // namespace application
