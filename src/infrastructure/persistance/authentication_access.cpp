#include "authentication_access.hpp"

namespace infrastructure::persistence
{

void AuthenticationAccess::authenticateUser(adapters::dtos::LoginDto loginDto)
{
    QUrl endpoint("https://localhost:7084/api/login");
    auto request = createRequest(endpoint);
        
    QJsonObject jsonObject;
    jsonObject["email"] = loginDto.email;
    jsonObject["password"] = loginDto.password;
    
    QJsonDocument jsonDocument{jsonObject};
    QByteArray data = jsonDocument.toJson();
    
    m_reply.reset(m_networkAccessManager.post(request, data));
    QObject::connect(m_reply.get(), &QNetworkReply::finished, this, &AuthenticationAccess::authenticationFinished);
}

void AuthenticationAccess::createUser(adapters::dtos::RegisterDto registerDto)
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
    QObject::connect(m_reply.get(), &QNetworkReply::finished, this, &AuthenticationAccess::creationFinished);
}


bool AuthenticationAccess::checkForErrors(int expectedStatusCode)
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

QNetworkRequest AuthenticationAccess::createRequest(QUrl url)
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


void AuthenticationAccess::authenticationFinished()
{
    auto expectedStatusCode = 200;
    if(checkForErrors(expectedStatusCode))
    {
        emit authenticationResponseReceived(false, "");
        return;
    }
    
    QString result = m_reply->readAll();
    emit authenticationResponseReceived(true, result);
}

void AuthenticationAccess::creationFinished()
{
    auto expectedStatusCode = 201;
    if(checkForErrors(expectedStatusCode))
    {
        QString reason = m_reply->readAll();
        emit userCreationResponseReceived(false, reason);
        return;
    }
    
    emit userCreationResponseReceived(true, "");
}

} // namespace infrastructure::persistence