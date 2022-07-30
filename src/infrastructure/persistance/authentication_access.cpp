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
    QObject::connect(m_reply.get(), &QNetworkReply::finished, this, &AuthenticationAccess::processResult);
}

void AuthenticationAccess::processResult()
{
    int statusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statusCode != 200)
    {
        qDebug() << "there was an error! " << m_reply->readAll();
        emit responseReceived("");
        return;
    }
    
    QString result = m_reply->readAll();
    emit responseReceived(result);
}

} // namespace infrastructure::persistence