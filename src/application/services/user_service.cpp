#include "user_service.hpp"
#include <QBuffer>
#include <QDebug>
#include <QImageReader>
#include <QStandardPaths>
#include "automatic_login_helper.hpp"
#include "tag.hpp"

using domain::entities::Tag;
using domain::entities::User;

namespace application::services
{

UserService::UserService(IUserStorageGateway* userStorageGateway) :
    m_userStorageGateway(userStorageGateway),
    m_user("x", "z", "s", 0, 0)
{
    connect(m_userStorageGateway, &IUserStorageGateway::authTokenExpired, this,
            &UserService::logoutUser);

    connect(m_userStorageGateway, &IUserStorageGateway::finishedGettingUser,
            this, &UserService::proccessUserInformation);

    connect(m_userStorageGateway, &IUserStorageGateway::profilePictureReady,
            this,
            [this](QByteArray& data)
            {
                auto path = saveProfilePictureToFile(data);
                m_user.setProfilePictureLastUpdated(
                    QDateTime::currentDateTimeUtc());
                m_user.setHasProfilePicture(true);
                updateProfilePictureUI(path);
            });

    connect(m_userStorageGateway, &IUserStorageGateway::passwordChangeFinished,
            this, &UserService::passwordChangeFinished);


    // Tag insertion
    connect(&m_user, &User::tagInsertionStarted, this,
            &UserService::tagInsertionStarted);
    connect(&m_user, &User::tagInsertionEnded, this,
            &UserService::tagInsertionEnded);

    // Tag deletion
    connect(&m_user, &User::tagDeletionStarted, this,
            &UserService::tagDeletionStarted);
    connect(&m_user, &User::tagDeletionEnded, this,
            &UserService::tagDeletionEnded);

    // Tag changed
    connect(&m_user, &User::tagsChanged, this, &UserService::tagsChanged);


    // The timer which controls in which intervall we will check for new changes
    // to the user data on the server.
    m_fetchChangesTimer.setInterval(m_fetchChangesInverval);
    connect(&m_fetchChangesTimer, &QTimer::timeout, this,
            [this]()
            {
                m_userStorageGateway->getUser(m_authenticationToken);
            });
}

void UserService::loadUser(bool rememberUser)
{
    auto success = tryLoadingUserFromFile();

    // When user was loaded from file, we know that 'rememberMe' is true
    m_rememberUser = success ? true : rememberUser;

    // For offline mode, skip server call and create default user if needed
    if (m_authenticationToken == "offline_token") {
        if (!success) {
            // Create default offline user
            m_user.setName("Offline User");
            m_user.setEmail("offline_user@local");
            m_user.setRole("user");
            m_user.setUsedBookStorage(0);
            m_user.setBookStorageLimit(100 * 1024 * 1024); // 100 MB
            m_user.setHasProfilePicture(false);
            
            emit bookStorageDataUpdated(m_user.getUsedBookStorage(),
                                        m_user.getBookStorageLimit());
        }
        emit finishedLoadingUser(true);
        return;
    }

    m_userStorageGateway->getUser(m_authenticationToken);
}

void UserService::deleteUser()
{
    // If the user has automatic login enabled, delete the data to avoid
    // logging into a non-existing account.
    auto userData = utility::AutomaticLoginHelper::tryAutomaticUserLoading();
    if(userData.has_value() && userData.value().email == m_user.getEmail())
    {
        utility::AutomaticLoginHelper::clearAutomaticLoginData();
    }

    deleteAllLocalUserData();

    m_userStorageGateway->deleteUser(m_authenticationToken);
}

void UserService::downloadUser()
{
    m_userStorageGateway->getUser(m_authenticationToken);
}

QString UserService::getName() const
{
    return m_user.getName();
}

void UserService::setName(const QString& newName)
{
    m_user.setName(newName);
    m_userStorageGateway->changeName(m_authenticationToken, m_user.getName());
}

QString UserService::getEmail() const
{
    return m_user.getEmail();
}

void UserService::setEmail(const QString& newEmail)
{
    m_user.setEmail(newEmail);
    m_userStorageGateway->changeEmail(m_authenticationToken, m_user.getEmail());
}

QString UserService::getRole() const
{
    return m_user.getRole();
}

qint64 UserService::getUsedBookStorage() const
{
    return m_user.getUsedBookStorage();
}

qint64 UserService::getBookStorageLimit() const
{
    return m_user.getBookStorageLimit();
}

QString UserService::getProfilePicturePath() const
{
    return m_user.getProfilePicturePath();
}

void UserService::setProfilePicturePath(const QString& path)
{
    if(path == m_user.getProfilePicturePath())
        return;

    auto savedProfilePicturePath = saveProfilePictureToFile(path);
    auto newProfilePictureLastUpdated = QDateTime::currentDateTimeUtc();

    m_user.setProfilePicturePath(savedProfilePicturePath);
    m_user.setProfilePictureLastUpdated(newProfilePictureLastUpdated);
    m_user.setHasProfilePicture(true);

    m_userStorageGateway->changeProfilePicture(m_authenticationToken,
                                               savedProfilePicturePath);
    m_userStorageGateway->changeProfilePictureLastUpdated(
        m_authenticationToken, newProfilePictureLastUpdated);
    m_userStorageGateway->changeHasProfilePicture(m_authenticationToken, true);

    updateProfilePictureUI(savedProfilePicturePath);
}

void UserService::deleteProfilePicture()
{
    auto newProfilePictureLastUpdated = deleteProfilePictureLocally();

    m_userStorageGateway->deleteProfilePicture(m_authenticationToken);
    m_userStorageGateway->changeProfilePictureLastUpdated(
        m_authenticationToken, newProfilePictureLastUpdated);
    m_userStorageGateway->changeHasProfilePicture(m_authenticationToken, false);
}

void UserService::changePassword(const QString& newPassword)
{
    m_userStorageGateway->changePassword(m_authenticationToken, newPassword);
}

void UserService::forgotPassword(const QString& email)
{
    m_userStorageGateway->forgotPassword(email);
}

QString UserService::saveProfilePictureToFile(QByteArray& data)
{
    auto userDir = getUserProfileDir();
    auto imageFormat = getImageFormat(data);
    auto path = userDir.filePath(m_profilePictureFileName + "." + imageFormat);

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "Could not open downloaded profile picture";
        return "";
    }

    file.write(data);

    // Manually close to make sure the data is written to file before
    // continuing
    file.close();

    return path;
}

QString UserService::saveProfilePictureToFile(const QString& path)
{
    QFile profilePicture(path);
    if(!profilePicture.open(QFile::ReadOnly))
    {
        qWarning() << "Could not open new profile picture";
        return "";
    }

    // Make sure thata 'readAll()' is stored to a variable to be an lvalue, else
    // it will bind to the QString overload and the method will call itself.
    auto data = profilePicture.readAll();
    return saveProfilePictureToFile(data);
}

QString UserService::getImageFormat(QByteArray& image) const
{
    QBuffer buffer(&image);
    buffer.open(QIODevice::ReadOnly);

    QImageReader imageReader(&buffer);
    QString format = imageReader.format();
    buffer.close();
    if(format.isEmpty())
    {
        qWarning()
            << "Failed reading the format of the downloaded profile picture";
        return "";
    }

    return format;
}

void UserService::loadProfilePictureFromFile()
{
    auto userDir = getUserProfileDir();

    QString fullProfilePictureName = getFullProfilePictureName();
    if(fullProfilePictureName.isEmpty())
        return;

    QString path = userDir.filePath(fullProfilePictureName);
    m_user.setProfilePicturePath(path);
    m_user.setHasProfilePicture(true);
}

void UserService::updateProfilePictureUI(const QString& path)
{
    // Update the image by first setting it to an invalid path, and
    // after to the right path, else the update doesnt work properly
    m_user.setProfilePicturePath("");
    emit profilePictureChanged();

    m_user.setProfilePicturePath(path);
    emit profilePictureChanged();
}

QString UserService::getFullProfilePictureName()
{
    auto userDir = getUserProfileDir();
    auto matchingFiles = userDir.entryList({ m_profilePictureFileName + ".*" });

    if(matchingFiles.isEmpty())
        return "";

    return matchingFiles.first();
}

QDateTime UserService::deleteProfilePictureLocally()
{
    auto newProfilePictureLastUpdated = QDateTime::currentDateTimeUtc();

    QString fullProfilePictureName = getFullProfilePictureName();
    if(fullProfilePictureName.isEmpty())
        return newProfilePictureLastUpdated;

    auto userDir = getUserProfileDir();
    QString path = userDir.filePath(fullProfilePictureName);

    QFile file(path);
    file.remove();

    m_user.setProfilePictureLastUpdated(newProfilePictureLastUpdated);
    m_user.setHasProfilePicture(false);

    updateProfilePictureUI("");

    // return date and time that have been set locally so app and server could
    // sync correctly
    return newProfilePictureLastUpdated;
}

const std::vector<domain::entities::Tag>& UserService::getTags() const
{
    return m_user.getTags();
}

QUuid UserService::addTag(const domain::entities::Tag& tag)
{
    auto success = m_user.addTag(tag);
    if(!success)
    {
        qWarning() << QString("Failed adding tag with name: %1 to user.")
                          .arg(tag.getName());
        return QUuid();
    }

    return tag.getUuid();
}

bool UserService::deleteTag(const QUuid& uuid)
{
    auto success = m_user.deleteTag(uuid);
    if(!success)
    {
        qWarning() << QString("Failed deleting tag with uuid: %1 from user.")
                          .arg(uuid.toString());
        return false;
    }

    m_userStorageGateway->deleteTag(m_authenticationToken, uuid);
    return true;
}

bool UserService::renameTag(const QUuid& uuid, const QString& newName)
{
    auto success = m_user.renameTag(uuid, newName);
    if(!success)
    {
        qWarning() << QString("Failed renaming tag with uuid: %1 with new "
                              "name: %2 from user.")
                          .arg(uuid.toString(), newName);
        return false;
    }

    m_userStorageGateway->renameTag(m_authenticationToken, uuid,
                                    m_user.getTagByUuid(uuid)->getName());
    return true;
}

void UserService::proccessUserInformation(const domain::entities::User& user,
                                          bool success)
{
    // Avoid storing data for logged out users by verifying login status before
    // applying data, else their data might be in memory even though logged out.
    if(!userIsLoggedIn() || !success)
    {
        emit finishedLoadingUser(false);
        return;
    }

    updateProfilePicture(user);
    setUserData(user);
    emit finishedLoadingUser(true);
    emit bookStorageDataUpdated(user.getUsedBookStorage(),
                                user.getBookStorageLimit());

    // If "rememberUser" is true, update the saved autologin user data
    // everytime there are changes to the user. This way, when logging in
    // via autologin the next time, you already have the newest changes
    // available.
    if(m_rememberUser)
        saveUserToFile(user);
}

void UserService::setUserData(const User& user)
{
    m_user.setName(user.getName());
    m_user.setEmail(user.getEmail());
    m_user.setRole(user.getRole());
    m_user.setUsedBookStorage(user.getUsedBookStorage());
    m_user.setBookStorageLimit(user.getBookStorageLimit());
    m_user.setProfilePictureLastUpdated(user.getProfilePictureLastUpdated());

    for(const auto& tag : user.getTags())
    {
        m_user.addTag(tag);
    }
}

void UserService::updateProfilePicture(const domain::entities::User& user)
{
    if(m_user.hasProfilePicture() && !user.hasProfilePicture())
    {
        deleteProfilePictureLocally();
    }
    else if(m_user.getProfilePictureLastUpdated().toSecsSinceEpoch() <
            user.getProfilePictureLastUpdated().toSecsSinceEpoch())
    {
        m_userStorageGateway->getProfilePicture(m_authenticationToken);
    }
}

void UserService::deleteAllLocalUserData()
{
    QDir profileDir(getUserProfileDir());
    if(!profileDir.exists())
    {
        qWarning() << "Can't delete user profile directory, no such directory.";
        return;
    }

    profileDir.removeRecursively();
}

bool UserService::userIsLoggedIn()
{
    return !m_authenticationToken.isEmpty();
}

bool UserService::tryLoadingUserFromFile()
{
    auto result = utility::AutomaticLoginHelper::tryAutomaticUserLoading();
    if(result.has_value())
    {
        utility::UserData userData = result.value();
        User user(userData.name, userData.email, userData.role,
                  userData.usedBookStorage, userData.bookStorageLimit);
        user.setProfilePictureLastUpdated(userData.profilePictureLastUpdated);
        for(auto& tag : userData.tags)
            user.addTag(tag);

        setUserData(user);
        loadProfilePictureFromFile();
        QString path = m_user.getProfilePicturePath();  // Needs to copy!
        updateProfilePictureUI(path);

        emit finishedLoadingUser(true);
        emit bookStorageDataUpdated(user.getUsedBookStorage(),
                                    user.getBookStorageLimit());

        return true;
    }

    return false;
}

void UserService::saveUserToFile(const domain::entities::User& user)
{
    utility::UserData userData {
        user.getName(),
        user.getEmail(),
        user.getRole(),
        user.getUsedBookStorage(),
        user.getBookStorageLimit(),
        user.getProfilePictureLastUpdated(),
        user.getTags(),
    };

    utility::AutomaticLoginHelper::saveUserData(userData);
}

void UserService::setupUserData(const QString& token, const QString& email)
{
    Q_UNUSED(email);
    m_authenticationToken = token;

    // Don't start periodic fetching for offline mode
    if (token != "offline_token") {
        m_fetchChangesTimer.start();
    }
}

void UserService::clearUserData()
{
    m_fetchChangesTimer.stop();
    m_user.clearData();
    m_authenticationToken.clear();
    m_rememberUser = false;
}

QDir UserService::getUserProfileDir() const
{
    QDir destDir(
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if(!destDir.exists())
        destDir.mkpath(".");

    destDir.mkdir("profiles");
    destDir.cd("profiles");

    auto userProfileHash = QString::number(qHash(m_user.getEmail()));
    destDir.mkdir(userProfileHash);
    destDir.cd(userProfileHash);

    return destDir;
}

}  // namespace application::services
