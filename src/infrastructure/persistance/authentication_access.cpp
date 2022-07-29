#include "authentication_access.hpp"

namespace infrastructure::persistence
{

QString AuthenticationAccess::loginUser(adapters::dtos::LoginDto loginDto)
{

    QNetworkRequest request{ QUrl("https://localhost:7084/api/login") };
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Version", "1.0");
    
    QSslConfiguration sslConfiguration = request.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    request.setSslConfiguration(sslConfiguration);
        
    QJsonObject jsonObject;
    jsonObject["email"] = "DavidLazarescu";
    jsonObject["password"] = "David12345";
    
    QJsonDocument jsonDocument{jsonObject};
    QByteArray data = jsonDocument.toJson();
    
    m_reply.reset(m_networkAccessManager.post(request, data));
    QObject::connect(m_reply.get(), &QNetworkReply::finished, this, &AuthenticationAccess::printResult);
    
    return "";
}
void AuthenticationAccess::printResult()
{
    if(m_reply->error() != QNetworkReply::NoError)
        qDebug() << "there was an error! " << m_reply->errorString();
        
    qDebug() << m_reply->readAll();
}

} // namespace infrastructure::persistence