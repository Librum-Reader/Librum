#include "user_storage_access.hpp"
#include <QFile>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include "api_error_helper.hpp"
#include "endpoints.hpp"

namespace infrastructure::persistence
{

void UserStorageAccess::getUser(const QString& authToken)
{
    auto request = createRequest(data::userGetEndpoint, authToken);
    auto reply = m_networkAccessManager.get(request);

    connect(reply, &QNetworkReply::finished, this,
            &UserStorageAccess::proccessGetUserResult);
}

void UserStorageAccess::getProfilePicture(const QString& authToken)
{
    QString endpoint = data::userProfilePictureEndpoint;
    auto request = createRequest(endpoint, authToken);
    auto pictureDownloadreply = m_networkAccessManager.get(request);

    connect(pictureDownloadreply, &QNetworkReply::finished, this,
            [this]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(
                        reply, "Downloading profile picture");

                    reply->deleteLater();
                    return;
                }

                QByteArray replyData = reply->readAll();
                emit profilePictureReady(replyData);

                reply->deleteLater();
            });
}

void UserStorageAccess::changeFirstName(const QString& authToken,
                                        const QString& newFirstName)
{
    auto request = createRequest(data::userPatchEndpoint, authToken);

    const QString quote = "\"";
    auto jsonData = R"([{ "op": "replace", "path": "firstName", "value": )" +
                    quote + newFirstName + quote + "}]";

    auto reply = m_networkAccessManager.sendCustomRequest(request, "PATCH",
                                                          jsonData.toUtf8());


    // Make sure to release the reply's memory
    connect(reply, &QNetworkReply::finished, this,
            [this]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply,
                                                      "Changing firstname");
                }

                reply->deleteLater();
            });
}

void UserStorageAccess::changeLastName(const QString& authToken,
                                       const QString& newLastName)
{
    auto request = createRequest(data::userPatchEndpoint, authToken);

    const QString quote = "\"";
    auto jsonData = R"([{ "op": "replace", "path": "lastName", "value": )" +
                    quote + newLastName + quote + "}]";

    auto reply = m_networkAccessManager.sendCustomRequest(request, "PATCH",
                                                          jsonData.toUtf8());


    // Make sure to release the reply's memory
    connect(reply, &QNetworkReply::finished, this,
            [this]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply,
                                                      "Changing lastname");
                }

                reply->deleteLater();
            });
}

void UserStorageAccess::changeEmail(const QString& authToken,
                                    const QString& newEmail)
{
    Q_UNUSED(authToken)
    Q_UNUSED(newEmail)
    // TODO: Implement
}

void UserStorageAccess::changeProfilePicture(const QString& authToken,
                                             const QString& path)
{
    auto profilePicture = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    addImagePartToMultiPart(profilePicture, path);


    QUrl endpoint = data::userProfilePictureEndpoint;
    auto request = createRequest(endpoint, authToken);

    // Reset the ContentTypeHeader since it will be set by the multipart
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArray());

    auto profilePictureUploadreply =
        m_networkAccessManager.post(request, profilePicture);

    // Make sure to free the data used for uploading the profile picture
    connect(profilePictureUploadreply, &QNetworkReply::finished, this,
            [this, profilePicture]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(
                        reply, "Uploading profile picture");
                }

                reply->deleteLater();
                profilePicture->deleteLater();
            });
}

void UserStorageAccess::deleteProfilePicture(const QString& authToken)
{
    QString endpoint = data::userProfilePictureEndpoint;
    auto request = createRequest(endpoint, authToken);
    auto pictureDeleteReply =
        m_networkAccessManager.sendCustomRequest(request, "DELETE");

    connect(pictureDeleteReply, &QNetworkReply::finished, this,
            [this]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(
                        reply, "Deleting profile picture");

                    reply->deleteLater();
                    return;
                }

                reply->deleteLater();
            });
}

void UserStorageAccess::changeProfilePictureLastUpdated(
    const QString& authToken, const QString& newDateTime)
{
    QString endPoint = data::userPatchEndpoint;
    auto request = createRequest(endPoint, authToken);

    const QString quote = "\"";
    auto jsonData =
        R"([{ "op": "replace", "path": "profilePictureLastUpdated", "value": )" +
        quote + newDateTime + quote + "}]";

    auto reply = m_networkAccessManager.sendCustomRequest(request, "PATCH",
                                                          jsonData.toUtf8());

    connect(reply, &QNetworkReply::finished, this,
            [this]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(
                        reply, "Changing profile picture last updated date");
                }

                reply->deleteLater();
            });
}

void UserStorageAccess::changeHasProfilePicture(const QString& authToken,
                                                const QString& newValue)
{
    QString endPoint = data::userPatchEndpoint;
    auto request = createRequest(endPoint, authToken);

    const QString quote = "\"";
    auto jsonData =
        R"([{ "op": "replace", "path": "hasProfilePicture", "value": )" +
        quote + newValue + quote + "}]";

    auto reply = m_networkAccessManager.sendCustomRequest(request, "PATCH",
                                                          jsonData.toUtf8());

    connect(reply, &QNetworkReply::finished, this,
            [this]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(
                        reply, "Changing has profile picture");
                }

                reply->deleteLater();
            });
}

void UserStorageAccess::deleteTag(const QString& authToken, const QString& uuid)
{
    QString endPoint = data::tagDeletionEndpoint + "/" + uuid;
    auto request = createRequest(endPoint, authToken);

    auto reply = m_networkAccessManager.sendCustomRequest(request, "DELETE");


    // Make sure to release the reply's memory
    connect(reply, &QNetworkReply::finished, this,
            [this]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 204))
                {
                    api_error_helper::logErrorMessage(reply, "Deleting tag");
                }

                reply->deleteLater();
            });
}

void UserStorageAccess::renameTag(const QString& authToken,
                                  const QJsonObject& bookForUpdate)
{
    QString endPoint = data::tagUpdateEndpoint;
    auto request = createRequest(endPoint, authToken);

    QJsonDocument jsonDoc(bookForUpdate);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    auto reply =
        m_networkAccessManager.sendCustomRequest(request, "PUT", jsonData);


    // Make sure to release the reply's memory
    connect(reply, &QNetworkReply::finished, this,
            [this]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 201))
                {
                    api_error_helper::logErrorMessage(reply, "Renaming tag");
                }

                reply->deleteLater();
            });
}

void UserStorageAccess::proccessGetUserResult()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    if(api_error_helper::apiRequestFailed(reply, 200))
    {
        api_error_helper::logErrorMessage(reply, "Getting User");

        emit gettingUserFailed();
        reply->deleteLater();
        return;
    }


    auto jsonDoc = QJsonDocument::fromJson(reply->readAll());
    auto jsonObj = jsonDoc.object();

    auto firstName = jsonObj["firstName"].toString();
    auto lastName = jsonObj["lastName"].toString();
    auto usedBookStorage =
        static_cast<long>(jsonObj["usedBookStorage"].toDouble());
    auto bookStorageLimit =
        static_cast<long>(jsonObj["bookStorageLimit"].toDouble());
    auto profilePictureLastUpdated = QDateTime::fromString(
        jsonObj["profilePictureLastUpdated"].toString(), Qt::ISODateWithMs);
    auto hasProfilePicture = jsonObj["hasProfilePicture"].toBool();
    auto email = jsonObj["email"].toString();
    auto tags = jsonObj["tags"].toArray();

    emit userReady(firstName, lastName, email, usedBookStorage,
                   bookStorageLimit, profilePictureLastUpdated,
                   hasProfilePicture, tags);

    // Make sure to release the reply's memory
    reply->deleteLater();
}

void UserStorageAccess::addImagePartToMultiPart(QHttpMultiPart* multiPart,
                                                const QString& imagePath)
{
    QFile* file = new QFile(QUrl(imagePath).path(), multiPart);
    if(!file->open(QIODevice::ReadOnly))
    {
        qWarning() << "Could not open new profile picture file: "
                   << file->fileName();

        multiPart->deleteLater();
        return;
    }

    QFileInfo fileInfo(imagePath);
    QHttpPart imagePart;
    imagePart.setBodyDevice(file);
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader,
                        QVariant("image/" + fileInfo.suffix()));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant("form-data; name=\"image\"; filename=\"" +
                                 file->fileName() + "\""));

    multiPart->append(imagePart);
}

QNetworkRequest UserStorageAccess::createRequest(const QUrl& url,
                                                 const QString& authToken)
{
    QNetworkRequest result { url };
    result.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    result.setRawHeader(QByteArray("Authorization"),
                        "Bearer " + authToken.toUtf8());

    QSslConfiguration sslConfiguration = result.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    result.setSslConfiguration(sslConfiguration);

    return result;
}

}  // namespace infrastructure::persistence
