#include <QObject>
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
    Q_PROPERTY(QString profilePicture READ getProfilePicture WRITE
                   setProfilePicture NOTIFY profilePictureChanged)

public:
    virtual ~IUserController() noexcept = default;

    Q_INVOKABLE virtual void loadUser() = 0;

    virtual QString getFirstName() = 0;
    virtual void setFirstName(const QString& newFirstName) = 0;

    virtual QString getLastName() = 0;
    virtual void setLastName(const QString& newLastName) = 0;

    virtual QString getEmail() = 0;
    virtual void setEmail(const QString& newEmail) = 0;

    virtual QString getProfilePicture() const = 0;
    virtual void setProfilePicture(const QString& path) = 0;

signals:
    void finishedLoadingUser(bool success);
    void firstNameChanged();
    void lastNameChanged();
    void emailChanged();
    void profilePictureChanged();
};

}  // namespace adapters