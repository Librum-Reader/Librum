#include "internet_connection_info.hpp"
#include <QDebug>
#include <QSslConfiguration>


namespace application::utility
{

void utility::InternetConnectionInfo::checkAvailability()
{
    QNetworkRequest request(QUrl("https://localhost:7084/api"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Version", "1.0");
    QSslConfiguration sslConfiguration = request.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    request.setSslConfiguration(sslConfiguration);
    
    m_internetCheckReply.reset(networkManager.get(request));
    QObject::connect(m_internetCheckReply.get(), &QNetworkReply::finished, 
                     this, &InternetConnectionInfo::proccessAvailabilityResult);
}

void InternetConnectionInfo::proccessAvailabilityResult()
{
    if(m_internetCheckReply->bytesAvailable())
        emit available();
}

}