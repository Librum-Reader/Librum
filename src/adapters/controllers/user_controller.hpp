#pragma once
#include <QObject>
#include <QString>
#include "i_user_controller.hpp"
#include "i_user_service.hpp"

namespace adapters::controllers
{

class UserController : public IUserController
{
    Q_OBJECT

public:
    UserController(application::IUserService* userService);

    void loadUser(bool rememberUser) override;
    
    void syncWithServer() override;

    QString getTagUuidForName(QString name) override;
    QString addTag(const QString& name) override;
    bool deleteTag(const QString& uuid) override;
    bool renameTag(const QString& uuid, const QString& newName) override;

    QString getFirstName() const override;
    void setFirstName(const QString& newFirstName) override;

    QString getLastName() const override;
    void setLastName(const QString& newLastName) override;

    QString getEmail() const override;
    void setEmail(const QString& newEmail) override;

    long getUsedBookStorage() const override;
    long getBookStorageLimit() const override;

    QString getProfilePicturePath() const override;
    void setProfilePicture(const QString& path) override;
    void deleteProfilePicture() override;

    data_models::UserTagsModel* getUserTagsModel() override;

private slots:
    void proccessUserLoadingResult(bool success);

private:
    application::IUserService* m_userService;
    data_models::UserTagsModel m_userTagsModel;
};

}  // namespace adapters::controllers
