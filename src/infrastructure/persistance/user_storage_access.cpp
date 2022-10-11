#include "user_storage_access.hpp"

namespace infrastructure::persistence
{

UserStorageAccess::UserStorageAccess()
    : m_getUserEndpoint("https://localhost:7084/api/user")
{
}

void UserStorageAccess::getUser(const QString& authenticationToken)
{
    auto request = createRequest(m_getUserEndpoint, authenticationToken);
    
    m_reply.reset(m_networkAccessManager.get(request));
    
    QObject::connect(m_reply.get(), &QNetworkReply::finished, 
                     this, &UserStorageAccess::proccessGetUserResult);
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
    QNetworkRequest result{ url };
    result.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    result.setRawHeader("X-Version", "1.0");
    result.setRawHeader(QByteArray("Authorization"), "Bearer " + authToken.toUtf8());
        
    QSslConfiguration sslConfiguration = result.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    result.setSslConfiguration(sslConfiguration);
    
    return result;
}

bool UserStorageAccess::checkForErrors(int expectedStatusCode)
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "there was an error! " << m_reply->errorString();
    }
    
    int statusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
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

} // namespace infrastructure::persistence