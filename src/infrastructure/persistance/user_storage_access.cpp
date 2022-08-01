#include "user_storage_access.hpp"

namespace infrastructure::persistence
{

void UserStorageAccess::authenticateUser(adapters::dtos::LoginDto loginDto)
{
    QUrl endpoint("https://localhost:7084/api/login");
    auto request = createRequest(endpoint);
        
    QJsonObject jsonObject;
    jsonObject["email"] = loginDto.email;
    jsonObject["password"] = loginDto.password;
    
    QJsonDocument jsonDocument{jsonObject};
    QByteArray data = jsonDocument.toJson();
    
    m_reply.reset(m_networkAccessManager.post(request, data));
    QObject::connect(m_reply.get(), &QNetworkReply::finished, this, &UserStorageAccess::authenticationFinished);
}

void UserStorageAccess::createUser(adapters::dtos::RegisterDto registerDto)
{
    QUrl endpoint("https://localhost:7084/api/register");
    auto request = createRequest(endpoint);
    
    QJsonObject jsonObject;
    jsonObject["firstName"] = registerDto.firstName;
    jsonObject["lastName"] = registerDto.lastName;
    jsonObject["email"] = registerDto.email;
    jsonObject["password"] = registerDto.password;
    
    QJsonDocument jsonDocument{jsonObject};
    QByteArray data = jsonDocument.toJson();
    
    m_reply.reset(m_networkAccessManager.post(request, data));
    QObject::connect(m_reply.get(), &QNetworkReply::finished, this, &UserStorageAccess::creationFinished);
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

QNetworkRequest UserStorageAccess::createRequest(QUrl url)
{
    QNetworkRequest result{ url };
    result.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    result.setRawHeader("X-Version", "1.0");
    
    QSslConfiguration sslConfiguration = result.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    result.setSslConfiguration(sslConfiguration);
    
    return result;
}


void UserStorageAccess::authenticationFinished()
{
    auto expectedStatusCode = 200;
    if(checkForErrors(expectedStatusCode))
    {
        emit authenticationResponseArrived(false, "");
        return;
    }
    
    QString result = m_reply->readAll();
    emit authenticationResponseArrived(true, result);
}

void UserStorageAccess::creationFinished()
{
    auto expectedStatusCode = 201;
    if(checkForErrors(expectedStatusCode))
    {
        QString reason = m_reply->readAll();
        emit userCreationResponseArrived(false, reason);
        return;
    }
    
    emit userCreationResponseArrived(true, "");
}

} // namespace infrastructure::persistence