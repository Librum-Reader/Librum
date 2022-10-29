#pragma once
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <memory>
#include "i_internet_connection_info.hpp"

namespace application::utility
{

class InternetConnectionInfo : public IInternetConnectionInfo
{
    Q_OBJECT

public:
    void checkAvailability() override;

private slots:
    void proccessAvailabilityResult();

private:
    std::unique_ptr<QNetworkReply> m_internetCheckReply = nullptr;
    QNetworkAccessManager networkManager;
};

}  // namespace application::utility