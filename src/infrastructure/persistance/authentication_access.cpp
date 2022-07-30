#include "authentication_access.hpp"

namespace infrastructure::persistence
{

void AuthenticationAccess::authenticateUser(adapters::dtos::LoginDto loginDto)
{
    QNetworkRequest request{ QUrl("https://localhost:7084/api/login") };
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Version", "1.0");
    
    QSslConfiguration sslConfiguration = request.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    request.setSslConfiguration(sslConfiguration);
        
    QJsonObject jsonObject;
    jsonObject["email"] = loginDto.email;
    jsonObject["password"] = loginDto.password;
    
    QJsonDocument jsonDocument{jsonObject};
    QByteArray data = jsonDocument.toJson();
    
    m_reply.reset(m_networkAccessManager.post(request, data));
    QObject::connect(m_reply.get(), &QNetworkReply::finished, this, &AuthenticationAccess::processAuthenticationResult);
}

void AuthenticationAccess::createUser(adapters::dtos::RegisterDto registerDto)
{
    QNetworkRequest request{ QUrl("https://localhost:7084/api/register") };
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Version", "1.0");
    
    QSslConfiguration sslConfiguration = request.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    request.setSslConfiguration(sslConfiguration);
        
    QJsonObject jsonObject;
    jsonObject["firstName"] = registerDto.firstName;
    jsonObject["lastName"] = registerDto.lastName;
    jsonObject["email"] = registerDto.email;
    jsonObject["password"] = registerDto.password;
    
    QJsonDocument jsonDocument{jsonObject};
    QByteArray data = jsonDocument.toJson();
    
    m_reply.reset(m_networkAccessManager.post(request, data));
    QObject::connect(m_reply.get(), &QNetworkReply::finished, this, &AuthenticationAccess::processCreationResult);
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


void AuthenticationAccess::processAuthenticationResult()
{
    if(checkForErrors(200))
    {
        emit authenticationResponseReceived(false, "");
        return;
    }
    
    QString result = m_reply->readAll();
    emit authenticationResponseReceived(true, result);
}

void AuthenticationAccess::processCreationResult()
{
    if(checkForErrors(201))
    {
        QString reason = m_reply->readAll();
        emit userCreationResponseReceived(false, reason);
        return;
    }
    
    emit userCreationResponseReceived(true, "");
}

} // namespace infrastructure::persistence