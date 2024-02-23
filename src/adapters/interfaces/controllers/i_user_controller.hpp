#pragma once
#include <QObject>
#include "adapters_export.hpp"
#include "user_tags_model.hpp"

namespace adapters
{

/**
 * The UserController class is exposed to the UI code and thus is the
 * "entry point" to the application's backend for user operations. It acts as a
 * layer of abstraction which maps the user data to a format usable for the
 * application.
 */
class ADAPTERS_EXPORT IUserController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString email READ getEmail WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(QString role READ getRole NOTIFY roleChanged)
    Q_PROPERTY(qint64 usedBookStorage READ getUsedBookStorage NOTIFY
                   usedBookStorageChanged CONSTANT)
    Q_PROPERTY(qint64 bookStorageLimit READ getBookStorageLimit NOTIFY
                   bookStorageLimitChanged CONSTANT)
    Q_PROPERTY(QString profilePicture READ getProfilePicturePath WRITE
                   setProfilePicture NOTIFY profilePictureChanged)
    Q_PROPERTY(adapters::data_models ::UserTagsModel* tagsModel READ
                   getUserTagsModel CONSTANT)

public:
    virtual ~IUserController() noexcept = default;

    Q_INVOKABLE virtual void loadUser(bool rememberUser) = 0;
    Q_INVOKABLE virtual void deleteUser() = 0;
    Q_INVOKABLE virtual void syncWithServer() = 0;
    Q_INVOKABLE virtual void deleteProfilePicture() = 0;
    Q_INVOKABLE virtual void changePassword(const QString& newPassword) = 0;
    Q_INVOKABLE virtual void forgotPassword(const QString& email) = 0;

    Q_INVOKABLE virtual QString getTagUuidForName(QString name) = 0;
    Q_INVOKABLE virtual QString addTag(const QString& name) = 0;
    Q_INVOKABLE virtual bool deleteTag(const QString& uuid) = 0;
    Q_INVOKABLE virtual bool renameTag(const QString& uuid,
                                       const QString& newName) = 0;

    virtual QString getName() const = 0;
    virtual void setName(const QString& newName) = 0;

    virtual QString getEmail() const = 0;
    virtual void setEmail(const QString& newEmail) = 0;

    virtual QString getRole() const = 0;

    virtual qint64 getUsedBookStorage() const = 0;
    virtual qint64 getBookStorageLimit() const = 0;

    virtual QString getProfilePicturePath() const = 0;
    virtual void setProfilePicture(const QString& path) = 0;

    virtual data_models::UserTagsModel* getUserTagsModel() = 0;

signals:
    void finishedLoadingUser(bool success);
    void nameChanged();
    void emailChanged();
    void roleChanged();
    void usedBookStorageChanged();
    void bookStorageLimitChanged();
    void profilePictureChanged();
    void passwordChangeFinished(bool success, const QString& reason);
};

}  // namespace adapters
