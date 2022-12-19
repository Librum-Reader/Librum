#pragma once
#include <QObject>
#include <QTimer>
#include "i_user_service.hpp"
#include "i_user_storage_gateway.hpp"
#include "user.hpp"

namespace application::services
{

class UserService : public IUserService
{
    Q_OBJECT

public:
    UserService(IUserStorageGateway* userStorageGateway);

    void loadUser() override;

    QString getFirstName() const override;
    void setFirstName(const QString& newFirstName) override;

    QString getLastName() const override;
    void setLastName(const QString& newLastName) override;

    QString getEmail() const override;
    void setEmail(const QString& newEmail) override;

    QImage getProfilePicture() const override;
    void setProfilePicture(const QImage& image) override;

    const std::vector<domain::entities::Tag>& getTags() const override;
    QUuid addTag(const domain::entities::Tag& tag) override;
    bool deleteTag(const QUuid& uuid) override;
    bool renameTag(const QUuid& uuid, const QString& newName) override;


public slots:
    void setupUserData(const QString& token,
                                const QString& email) override;
    void clearUserData() override;


private slots:
    void proccessUserInformation(const domain::entities::User& user,
                                 bool success);

private:
    IUserStorageGateway* m_userStorageGateway;
    domain::entities::User m_user;
    QString m_authenticationToken;
    QTimer m_fetchChangesTimer;
};

}  // namespace application::services