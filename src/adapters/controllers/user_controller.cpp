#include "user_controller.hpp"
#include <QBuffer>
#include <QByteArray>
#include <QDebug>
#include <QImageReader>
#include <QUrl>
#include "tag.hpp"

using domain::entities::Tag;

namespace adapters::controllers
{

UserController::UserController(application::IUserService* userService) :
    m_userService(userService),
    m_userTagsModel(m_userService->getTags())
{
    connect(m_userService, &application::IUserService::finishedLoadingUser,
            this, &UserController::proccessUserLoadingResult);

    connect(m_userService, &application::IUserService::profilePictureChanged,
            this, &UserController::profilePictureChanged);

    connect(m_userService, &application::IUserService::passwordChangeFinished,
            this, &UserController::passwordChangeFinished);


    // Tag insertion
    connect(m_userService, &application::IUserService::tagInsertionStarted,
            &m_userTagsModel, &data_models::UserTagsModel::startInsertingRow);

    connect(m_userService, &application::IUserService::tagInsertionEnded,
            &m_userTagsModel, &data_models::UserTagsModel::endInsertingRow);

    // Tag deletion
    connect(m_userService, &application::IUserService::tagDeletionStarted,
            &m_userTagsModel, &data_models::UserTagsModel::startDeletingRow);

    connect(m_userService, &application::IUserService::tagDeletionEnded,
            &m_userTagsModel, &data_models::UserTagsModel::endDeletingRow);

    // Tags changes
    connect(m_userService, &application::IUserService::tagsChanged,
            &m_userTagsModel, &data_models::UserTagsModel::refreshRows);
}

void UserController::loadUser(bool rememberUser)
{
    m_userService->loadUser(rememberUser);
}

void UserController::deleteUser()
{
    m_userService->deleteUser();
}

void UserController::syncWithServer()
{
    m_userService->downloadUser();
}

void UserController::changePassword(const QString& newPassword)
{
    m_userService->changePassword(newPassword);
}

void UserController::forgotPassword(const QString& email)
{
    m_userService->forgotPassword(email);
}

QString UserController::getTagUuidForName(QString name)
{
    // Tags are always capitalized, make sure to search for the correct one
    name[0] = name[0].toUpper();

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

bool UserController::deleteTag(const QString& uuid)
{
    auto result = m_userService->deleteTag(QUuid(uuid));
    return result;
}

bool UserController::renameTag(const QString& uuid, const QString& newName)
{
    auto result = m_userService->renameTag(QUuid(uuid), newName);
    return result;
}

QString UserController::getName() const
{
    return m_userService->getName();
}

void UserController::setName(const QString& newName)
{
    if(newName == m_userService->getName())
        return;

    m_userService->setName(newName);
    emit nameChanged();
}

QString UserController::getEmail() const
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

QString UserController::getRole() const
{
    return m_userService->getRole();
}

qint64 UserController::getUsedBookStorage() const
{
    return m_userService->getUsedBookStorage();
}

qint64 UserController::getBookStorageLimit() const
{
    return m_userService->getBookStorageLimit();
}

QString UserController::getProfilePicturePath() const
{
    auto path = m_userService->getProfilePicturePath();
    auto pathWithScheme = QUrl::fromLocalFile(path).toString();

    return path.isEmpty() ? "" : pathWithScheme;
}

void UserController::setProfilePicture(const QString& path)
{
    QString localPath = QUrl(path).toLocalFile();
    QImageReader imageReader(localPath);
    if(!QImageReader::supportedImageFormats().contains(imageReader.format()) ||
       imageReader.format() == "pdf")
    {
        qWarning() << "Provided profile picture format is not supported";
        return;
    }

    QImage profilePicture(localPath);
    if(profilePicture.isNull())
    {
        qWarning() << "Failed reading provided profile picture";
        return;
    }

    m_userService->setProfilePicturePath(localPath);
}

void UserController::deleteProfilePicture()
{
    m_userService->deleteProfilePicture();
}

data_models::UserTagsModel* UserController::getUserTagsModel()
{
    return &m_userTagsModel;
}

void UserController::proccessUserLoadingResult(bool success)
{
    emit nameChanged();
    emit emailChanged();
    emit usedBookStorageChanged();
    emit bookStorageLimitChanged();
    emit roleChanged();

    emit finishedLoadingUser(success);
}

}  // namespace adapters::controllers
