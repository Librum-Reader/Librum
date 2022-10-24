#pragma once
#include <memory>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "i_internet_connection_info.hpp"


namespace application::utility
{

class InternetConnectionInfo : public IInternetConnectionInfo
{
    Q_OBJECT
    
public:
    void checkAvailability() override;
    
private:
    std::unique_ptr<QNetworkReply> m_internetCheckReply = nullptr;
    QNetworkAccessManager networkManager;
};

} // namespace application::utility