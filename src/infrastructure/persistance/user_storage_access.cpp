#include "user_storage_access.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include "endpoints.hpp"

namespace infrastructure::persistence
{

void UserStorageAccess::getUser(const QString& authToken)
{
    auto request = createRequest(data::patchUserEndpoint, authToken);
    m_reply.reset(m_networkAccessManager.get(request));

    connect(m_reply.get(), &QNetworkReply::finished, this,
            &UserStorageAccess::proccessGetUserResult);
}

void UserStorageAccess::changeFirstName(const QString& authToken,
                                        const QString& newFirstName)
{
    auto request = createRequest(data::patchUserEndpoint, authToken);

    const QString quote = "\"";
    auto jsonData = R"([{ "op": "replace", "path": "firstName", "value": )" +
                    quote + newFirstName + quote + "}]";

    m_networkAccessManager.sendCustomRequest(request, "PATCH",
                                             jsonData.toUtf8());
}

void UserStorageAccess::changeLastName(const QString& authToken,
                                       const QString& newLastName)
{
    auto request = createRequest(data::patchUserEndpoint, authToken);

    const QString quote = "\"";
    auto jsonData = R"([{ "op": "replace", "path": "lastName", "value": )" +
                    quote + newLastName + quote + "}]";

    m_networkAccessManager.sendCustomRequest(request, "PATCH",
                                             jsonData.toUtf8());
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

void UserStorageAccess::addTag(const QString& authToken, const QJsonObject& tag)
{
    auto request = createRequest(data::addTagEndpoint, authToken);

    QJsonDocument jsonDocument(tag);
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    m_networkAccessManager.post(request, data);
}

void UserStorageAccess::removeTag(const QString& authToken, const QString& uuid)
{
    QString endPoint = data::addTagEndpoint + "/" + uuid;
    auto request = createRequest(endPoint, authToken);

    m_networkAccessManager.sendCustomRequest(request, "DELETE");
}

void UserStorageAccess::renameTag(const QString& authToken, const QString& uuid,
                                  const QString& newName)
{
    Q_UNUSED(authToken);
    Q_UNUSED(uuid);
    Q_UNUSED(newName);
}

void UserStorageAccess::proccessGetUserResult()
{
    int expectedStatusCode = 200;
    if(checkForErrors(expectedStatusCode))
    {
        emit gettingUserFailed();
        return;
    }

    auto jsonDoc = QJsonDocument::fromJson(m_reply->readAll());
    auto jsonObj = jsonDoc.object();

    auto firstName = jsonObj["firstName"].toString();
    auto lastName = jsonObj["lastName"].toString();
    auto email = jsonObj["email"].toString();
    auto tags = jsonObj["tags"].toArray();

    emit userReady(firstName, lastName, email, tags);
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

bool UserStorageAccess::checkForErrors(int expectedStatusCode)
{
    if(m_reply->error() != QNetworkReply::NoError)
        qDebug() << "there was an error! " << m_reply->errorString();

    int statusCode =
        m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statusCode != expectedStatusCode)
    {
        qDebug() << "there was an error! " << m_reply->readAll();
        return true;
    }

    return false;
}

}  // namespace infrastructure::persistence