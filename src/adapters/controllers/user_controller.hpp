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

    void loadUser() override;

    bool addTag(const QString& name) override;
    bool removeTag(const QString& name) override;
    bool renameTag(const QString& oldName, const QString& newName) override;

    QString getFirstName() override;
    void setFirstName(const QString& newFirstName) override;

    QString getLastName() override;
    void setLastName(const QString& newLastName) override;

    QString getEmail() override;
    void setEmail(const QString& newEmail) override;

    QString getProfilePicture() const override;
    void setProfilePicture(const QString& path) override;

    data_models::UserTagsModel* getUserTagsModel() override;

private slots:
    void proccessUserLoadingResult(bool success);

private:
    application::IUserService* m_userService;
    data_models::UserTagsModel m_userTagsModel;
};

}  // namespace adapters::controllers