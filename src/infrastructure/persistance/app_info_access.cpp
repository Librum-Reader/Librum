#include "app_info_access.hpp"
#include <QSettings>
#include "api_error_helper.hpp"
#include "endpoints.hpp"

namespace infrastructure::persistence
{

AppInfoAccess::AppInfoAccess()
{
    QSettings settings;
    domain = settings.value("serverHost").toString();
}

void AppInfoAccess::getNewestAppVersion()
{
    auto request = createRequest(domain + data::latestAppVersionEndpoint);
    auto reply = m_networkAccessManager.get(request);


    // Handle authentication result and release the reply's memory
    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply,
                                                      "Getting latest version");

                    emit newestAppVersionReceived("");
                    reply->deleteLater();
                    return;
                }

                emit newestAppVersionReceived(reply->readAll());
                reply->deleteLater();
            });
}

void AppInfoAccess::downloadBinaries(const QString& packageName)
{
    auto url = data::binaryDownloadEndpoint + "/" + packageName + ".zip";
    auto request = createRequest(url);
    auto reply = m_networkAccessManager.get(request);


    // Handle authentication result and release the reply's memory
    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply,
                                                      "Getting binary package");

                    emit downloadingBinariesFinished(QByteArray(), false);
                    reply->deleteLater();
                    return;
                }

                emit downloadingBinariesFinished(reply->readAll(), true);
                reply->deleteLater();
            });

    connect(reply, &QNetworkReply::downloadProgress, this,
            &AppInfoAccess::downloadingBinariesProgressChanged);
}

QNetworkRequest AppInfoAccess::createRequest(QUrl url)
{
    QNetworkRequest result { url };
    result.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QSslConfiguration sslConfiguration = result.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    result.setSslConfiguration(sslConfiguration);

    return result;
}

}  // namespace infrastructure::persistence
