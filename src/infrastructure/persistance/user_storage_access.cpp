#include "user_storage_access.hpp"

namespace infrastructure::persistence
{

UserStorageAccess::UserStorageAccess() :
    m_getUserEndpoint("https://localhost:7084/api/user"),
    m_patchUserEndpoint("https://localhost:7084/api/user")
{
}

void UserStorageAccess::getUser(const QString& authToken)
{
    auto request = createRequest(m_getUserEndpoint, authToken);

    m_reply.reset(m_networkAccessManager.get(request));

    connect(m_reply.get(), &QNetworkReply::finished, this,
            &UserStorageAccess::proccessGetUserResult);
}

void UserStorageAccess::changeFirstName(const QString& authToken,
                                        const QString& newFirstName)
{
    auto request = createRequest(m_patchUserEndpoint, authToken);

    const QString quote = "\"";
    auto jsonData = R"([{ "op": "replace", "path": "firstName", "value": )" +
                    quote + newFirstName + quote + "}]";

    m_networkAccessManager.sendCustomRequest(request, "PATCH",
                                             jsonData.toUtf8());
}

void UserStorageAccess::changeLastName(const QString& authToken,
                                       const QString& newLastName)
{
    auto request = createRequest(m_patchUserEndpoint, authToken);

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

void UserStorageAccess::proccessGetUserResult()
{
    int expectedStatusCode = 200;
    if(checkForErrors(expectedStatusCode))
    {
        emit gettingUserFailed();
        return;
    }

    auto valueMap = parseJsonToMap(m_reply->readAll());

    auto firstName = valueMap["firstName"].toString();
    auto lastName = valueMap["lastName"].toString();
    auto email = valueMap["email"].toString();

    emit userReady(firstName, lastName, email);
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

QVariantMap UserStorageAccess::parseJsonToMap(QByteArray jsonBytes)
{
    auto jsonResponse = QJsonDocument::fromJson(jsonBytes);
    if(jsonResponse.isNull() || !jsonResponse.isObject())
        emit gettingUserFailed();

    auto map = jsonResponse.object().toVariantMap();
    return map;
}

}  // namespace infrastructure::persistence