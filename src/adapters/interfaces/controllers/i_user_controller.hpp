#include <QObject>
#include "user_tags_model.hpp"
#pragma once

namespace adapters
{

/**
 * The UserController class is exposed to the UI code and thus is the
 * "entry point" to the application's backend for user operations. It acts as a
 * layer of abstraction which maps the user data to a format usable for the
 * application.
 */
class IUserController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString firstName READ getFirstName WRITE setFirstName NOTIFY
                   firstNameChanged)
    Q_PROPERTY(QString lastName READ getLastName WRITE setLastName NOTIFY
                   lastNameChanged)
    Q_PROPERTY(QString email READ getEmail WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(long usedBookStorage READ getUsedBookStorage NOTIFY
                   usedBookStorageChanged CONSTANT)
    Q_PROPERTY(long bookStorageLimit READ getBookStorageLimit NOTIFY
                   bookStorageLimitChanged CONSTANT)
    Q_PROPERTY(QString profilePicture READ getProfilePicturePath WRITE
                   setProfilePicture NOTIFY profilePictureChanged)
    Q_PROPERTY(adapters::data_models ::UserTagsModel* tagsModel READ
                   getUserTagsModel CONSTANT)

public:
    virtual ~IUserController() noexcept = default;

    Q_INVOKABLE virtual void loadUser(bool rememberUser) = 0;

    Q_INVOKABLE virtual void refresh() = 0;

    Q_INVOKABLE virtual QString getTagUuidForName(QString name) = 0;
    Q_INVOKABLE virtual QString addTag(const QString& name) = 0;
    Q_INVOKABLE virtual bool deleteTag(const QString& uuid) = 0;
    Q_INVOKABLE virtual bool renameTag(const QString& uuid,
                                       const QString& newName) = 0;

    virtual QString getFirstName() const = 0;
    virtual void setFirstName(const QString& newFirstName) = 0;

    virtual QString getLastName() const = 0;
    virtual void setLastName(const QString& newLastName) = 0;

    virtual QString getEmail() const = 0;
    virtual void setEmail(const QString& newEmail) = 0;

    virtual long getUsedBookStorage() const = 0;
    virtual long getBookStorageLimit() const = 0;

    virtual QString getProfilePicturePath() const = 0;
    virtual void setProfilePicture(const QString& path) = 0;
    virtual void deleteProfilePicture() = 0;

    virtual data_models::UserTagsModel* getUserTagsModel() = 0;

signals:
    void finishedLoadingUser(bool success);
    void firstNameChanged();
    void lastNameChanged();
    void emailChanged();
    void usedBookStorageChanged();
    void bookStorageLimitChanged();
    void profilePictureChanged();
};

}  // namespace adapters
