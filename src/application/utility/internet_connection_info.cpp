#include "internet_connection_info.hpp"


namespace application::utility
{

void utility::InternetConnectionInfo::checkAvailability()
{
    QNetworkRequest request(QUrl("https://localhost:7084"));
    m_internetCheckReply.reset(networkManager.get(request));
    QObject::connect(m_internetCheckReply.get(), &QNetworkReply::finished, 
                     this, &InternetConnectionInfo::available);
}

}