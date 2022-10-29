#pragma once
#include <QObject>
#include <QString>
#include "user.hpp"

namespace application
{

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
    void finishedGettingUser(domain::models::User user, bool success);
};

}  // namespace application