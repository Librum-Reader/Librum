#include "test_request.hpp"

TestRequest::TestRequest(QObject *parent)
    : QObject{parent}, m_reply{nullptr}
{
    
}

void TestRequest::makeRequest()
{
    QNetworkRequest request{ QUrl("https://localhost:5001/api/account/login") };
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QSslConfiguration sslConfiguration = request.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    request.setSslConfiguration(sslConfiguration);
    
    QJsonObject jsonObject;
    jsonObject["username"] = "DavidLazarescu";
    jsonObject["password"] = "12345";
    
    QJsonDocument jsonDocument{jsonObject};
    QByteArray data = jsonDocument.toJson();
    
    m_reply.reset(m_networkAccessManager.post(request, data));
    QObject::connect(m_reply.get(), &QNetworkReply::finished, this, &TestRequest::printResult);
}

void TestRequest::printResult()
{
    if(m_reply->error() != QNetworkReply::NoError)
        qDebug() << "there was an error! " << m_reply->errorString();
        
    qDebug() << m_reply->readAll();
}
