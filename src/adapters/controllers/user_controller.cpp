#include "user_controller.hpp"
#include <QBuffer>
#include <QByteArray>
#include <QUrl>
#include "tag.hpp"

using domain::models::Tag;

namespace adapters::controllers
{

UserController::UserController(application::IUserService* userService) :
    m_userService(userService),
    m_userTagsModel(m_userService->getTags())
{
    connect(m_userService, &application::IUserService::finishedLoadingUser,
            this, &UserController::proccessUserLoadingResult);


    // tag insertion
    connect(m_userService, &application::IUserService::tagInsertionStarted,
            &m_userTagsModel, &data_models::UserTagsModel::startInsertingRow);

    connect(m_userService, &application::IUserService::tagInsertionEnded,
            &m_userTagsModel, &data_models::UserTagsModel::endInsertingRow);

    connect(m_userService, &application::IUserService::tagDeletionStarted,
            &m_userTagsModel, &data_models::UserTagsModel::startDeletingRow);

    connect(m_userService, &application::IUserService::tagDeletionEnded,
            &m_userTagsModel, &data_models::UserTagsModel::endDeletingRow);

    // tags changed
    connect(m_userService, &application::IUserService::tagsChanged,
            &m_userTagsModel, &data_models::UserTagsModel::refreshRows);
}

void UserController::loadUser()
{
    m_userService->loadUser();
}

QString UserController::getTagUuidForName(const QString& name)
{
    for(const auto& tag : m_userService->getTags())
    {
        if(tag.getName() == name)
            return tag.getUuid().toString();
    }

    return "";
}

QString UserController::addTag(const QString& name)
{
    Tag tag(name);
    QUuid uuid = m_userService->addTag(tag);

    return uuid.toString();
}

bool UserController::removeTag(const QString& uuid)
{
    auto result = m_userService->removeTag(uuid);
    return result;
}

bool UserController::renameTag(const QString& uuid, const QString& newName)
{
    auto result = m_userService->renameTag(uuid, newName);
    return result;
}

QString UserController::getFirstName()
{
    return m_userService->getFirstName();
}

void UserController::setFirstName(const QString& newFirstName)
{
    if(newFirstName == m_userService->getFirstName())
        return;

    m_userService->setFirstName(newFirstName);
    emit firstNameChanged();
}

QString UserController::getLastName()
{
    return m_userService->getLastName();
}

void UserController::setLastName(const QString& newLastName)
{
    if(newLastName == m_userService->getLastName())
        return;

    m_userService->setLastName(newLastName);
    emit lastNameChanged();
}

QString UserController::getEmail()
{
    return m_userService->getEmail();
}

void UserController::setEmail(const QString& newEmail)
{
    if(newEmail == m_userService->getEmail())
        return;

    m_userService->setEmail(newEmail);
    emit emailChanged();
}

QString UserController::getProfilePicture() const
{
    auto profilePicture = m_userService->getProfilePicture();
    if(profilePicture.isNull())
        return QString("");


    QByteArray byteArray;
    QBuffer buffer(&byteArray);

    buffer.open(QIODevice::WriteOnly);
    profilePicture.save(&buffer, "png");
    QString base64 = QString::fromUtf8(byteArray.toBase64());

    return "data:image/png;base64," + base64;
}

void UserController::setProfilePicture(const QString& path)
{
    QUrl url(path);
    QImage profilePicture(url.toLocalFile());
    if(profilePicture == m_userService->getProfilePicture())
        return;

    m_userService->setProfilePicture(profilePicture);
    emit profilePictureChanged();
}

data_models::UserTagsModel* UserController::getUserTagsModel()
{
    return &m_userTagsModel;
}

void UserController::proccessUserLoadingResult(bool success)
{
    emit firstNameChanged();
    emit lastNameChanged();
    emit emailChanged();

    emit finishedLoadingUser(success);
}

}  // namespace adapters::controllers