#pragma once
#include <QObject>
#include <QString>
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

    virtual void changeFirstName(const QString& authToken,
                                 const QString& newFirstName) = 0;
    virtual void changeLastName(const QString& authToken,
                                const QString& newLastName) = 0;
    virtual void changeEmail(const QString& authToken,
                             const QString& newEmail) = 0;
    virtual void changeProfilePicture(const QString& authToken,
                                      const QImage& newPicture) = 0;

signals:
    void finishedGettingUser(const domain::models::User& user, bool success);
};

}  // namespace application