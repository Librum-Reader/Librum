#include "user_storage_access.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include "endpoints.hpp"

namespace infrastructure::persistence
{

void UserStorageAccess::getUser(const QString& authToken)
{
    auto request = createRequest(data::userGetEndpoint, authToken);
    m_getUserReply.reset(m_networkAccessManager.get(request));

    connect(m_getUserReply.get(), &QNetworkReply::finished, this,
            &UserStorageAccess::proccessGetUserResult);
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
    m_firstNameUpdateReply.reset(reply);

    linkRequestToErrorHandling(m_firstNameUpdateReply.get(), 200);
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
    m_lastNameUpdateReply.reset(reply);

    linkRequestToErrorHandling(m_lastNameUpdateReply.get(), 200);
}

void UserStorageAccess::changeEmail(const QString& authToken,
                                    const QString& newEmail)
{
    Q_UNUSED(authToken)
    Q_UNUSED(newEmail)
    // TODO: Implement
}

void UserStorageAccess::changeProfilePicture(const QString& authToken,
                                             const QImage& newProfilePicture)
{
    Q_UNUSED(authToken)
    Q_UNUSED(newProfilePicture)
    // TODO: Implement when filestorage server is up
}

void UserStorageAccess::deleteTag(const QString& authToken, const QString& uuid)
{
    QString endPoint = data::tagDeletionEndpoint + "/" + uuid;
    auto request = createRequest(endPoint, authToken);

    auto reply = m_networkAccessManager.sendCustomRequest(request, "DELETE");
    m_tagDeletionReply.reset(reply);

    linkRequestToErrorHandling(m_tagDeletionReply.get(), 204);
}

void UserStorageAccess::renameTag(const QString& authToken, const QString& uuid,
                                  const QJsonObject& bookForUpdate)
{
    QString endPoint = data::tagUpdateEndpoint + "/" + uuid;
    auto request = createRequest(endPoint, authToken);

    QJsonDocument jsonDoc(bookForUpdate);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    auto result =
        m_networkAccessManager.sendCustomRequest(request, "PUT", jsonData);
    m_tagRenameReply.reset(result);

    linkRequestToErrorHandling(m_tagRenameReply.get(), 201);
}

void UserStorageAccess::proccessGetUserResult()
{
    int expectedStatusCode = 200;
    if(checkForErrors(expectedStatusCode, m_getUserReply.get()))
    {
        emit gettingUserFailed();
        return;
    }

    auto jsonDoc = QJsonDocument::fromJson(m_getUserReply->readAll());
    auto jsonObj = jsonDoc.object();

    auto firstName = jsonObj["firstName"].toString();
    auto lastName = jsonObj["lastName"].toString();
    auto email = jsonObj["email"].toString();
    auto tags = jsonObj["tags"].toArray();

    emit userReady(firstName, lastName, email, tags);
}

void UserStorageAccess::linkRequestToErrorHandling(QNetworkReply* reply,
                                                   int statusCode)
{
    connect(reply, &QNetworkReply::finished, this,
            [this, statusCode, reply]()
            {
                checkForErrors(statusCode, reply);
            });
}

QNetworkRequest UserStorageAccess::createRequest(const QUrl& url,
                                                 const QString& authToken)
{
    QNetworkRequest result { url };
    result.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    result.setRawHeader("X-Version", "1.0");
    result.setRawHeader(QByteArray("Authorization"),
                        "Bearer " + authToken.toUtf8());

    QSslConfiguration sslConfiguration = result.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    result.setSslConfiguration(sslConfiguration);

    return result;
}

bool UserStorageAccess::checkForErrors(int expectedStatusCode,
                                       QNetworkReply* reply)
{
    auto statusCode =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(reply->error() != QNetworkReply::NoError ||
       expectedStatusCode != statusCode)
    {
        qWarning() << "User storage error: " << reply->errorString()
                   << "\nServer reply: " << reply->readAll();
        return true;
    }

    return false;
}

}  // namespace infrastructure::persistence