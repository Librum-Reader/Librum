#pragma once
#include <QObject>
#include <QTimer>
#include <QtCore/QtGlobal>
#include "application_export.hpp"
#include "i_user_service.hpp"
#include "i_user_storage_gateway.hpp"
#include "user.hpp"

namespace application::services
{

class APPLICATION_EXPORT UserService : public IUserService
{
    Q_OBJECT

public:
    UserService(IUserStorageGateway* userStorageGateway);

    void loadUser(bool rememberUser) override;
    void deleteUser() override;
    void downloadUser() override;

    QString getName() const override;
    void setName(const QString& newName) override;

    QString getEmail() const override;
    void setEmail(const QString& newEmail) override;

    QString getRole() const override;

    qint64 getUsedBookStorage() const override;

    qint64 getBookStorageLimit() const override;

    QString getProfilePicturePath() const override;
    void setProfilePicturePath(const QString& path) override;

    void deleteProfilePicture() override;
    void changePassword(const QString& newPassword) override;
    void forgotPassword(const QString& email) override;

    const std::vector<domain::entities::Tag>& getTags() const override;
    QUuid addTag(const domain::entities::Tag& tag) override;
    bool deleteTag(const QUuid& uuid) override;
    bool renameTag(const QUuid& uuid, const QString& newName) override;

public slots:
    void setupUserData(const QString& token, const QString& email) override;
    void clearUserData() override;


private slots:
    void proccessUserInformation(const domain::entities::User& user,
                                 bool success);

private:
    bool userIsLoggedIn();
    bool tryLoadingUserFromFile();
    void saveUserToFile(const domain::entities::User& user);
    QDir getUserProfileDir() const;
    QString saveProfilePictureToFile(QByteArray& data);
    QString saveProfilePictureToFile(const QString& path);
    QString getImageFormat(QByteArray& image) const;
    void loadProfilePictureFromFile();
    void updateProfilePictureUI(const QString& path);
    QString getFullProfilePictureName();
    QDateTime deleteProfilePictureLocally();
    void setUserData(const domain::entities::User& user);
    void updateProfilePicture(const domain::entities::User& user);
    void deleteAllLocalUserData();

    IUserStorageGateway* m_userStorageGateway;
    domain::entities::User m_user;
    QString m_profilePictureFileName = "profilePicture";
    QString m_authenticationToken;
    QTimer m_fetchChangesTimer;
    const int m_fetchChangesInverval = 15'000;
    bool m_rememberUser = false;
};

}  // namespace application::services
