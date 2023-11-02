#pragma once
#include <QByteArray>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include "i_app_info_access.hpp"

namespace infrastructure::persistence
{

class AppInfoAccess : public adapters::IAppInfoAccess
{
    Q_OBJECT

public:
    void getNewestAppVersion() override;
    void downloadBinaries(const QString& packageName) override;

private:
    QNetworkRequest createRequest(QUrl url);

    QNetworkAccessManager m_networkAccessManager;
};

}  // namespace infrastructure::persistence