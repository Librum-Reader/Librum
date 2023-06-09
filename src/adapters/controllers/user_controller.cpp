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


    // tag insertion
    connect(m_userService, &application::IUserService::tagInsertionStarted,
            &m_userTagsModel, &data_models::UserTagsModel::startInsertingRow);

    connect(m_userService, &application::IUserService::tagInsertionEnded,
            &m_userTagsModel, &data_models::UserTagsModel::endInsertingRow);

    // tag deletion
    connect(m_userService, &application::IUserService::tagDeletionStarted,
            &m_userTagsModel, &data_models::UserTagsModel::startDeletingRow);

    connect(m_userService, &application::IUserService::tagDeletionEnded,
            &m_userTagsModel, &data_models::UserTagsModel::endDeletingRow);

    // tags changes
    connect(m_userService, &application::IUserService::tagsChanged,
            &m_userTagsModel, &data_models::UserTagsModel::refreshRows);
}

void UserController::loadUser(bool rememberUser)
{
    m_userService->loadUser(rememberUser);
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
    auto result = m_userService->deleteTag(uuid);
    return result;
}

bool UserController::renameTag(const QString& uuid, const QString& newName)
{
    auto result = m_userService->renameTag(uuid, newName);
    return result;
}

QString UserController::getFirstName() const
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

QString UserController::getLastName() const
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

long UserController::getUsedBookStorage() const
{
    return m_userService->getUsedBookStorage();
}

long UserController::getBookStorageLimit() const
{
    return m_userService->getBookStorageLimit();
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
    QImageReader imageReader(url.path());
    if(imageReader.canRead())
    {
        qWarning() << "Provided profile picture is not an image";
        return;
    }

    QImage profilePicture(url.path());
    if(profilePicture.isNull())
    {
        qWarning() << "Failed reading provided profile picture";
        return;
    }

    m_userService->setProfilePicture(path, profilePicture);
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
    emit usedBookStorageChanged();
    emit bookStorageLimitChanged();

    emit finishedLoadingUser(success);
}

}  // namespace adapters::controllers
