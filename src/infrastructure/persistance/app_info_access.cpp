#pragma once
#include "app_info_access.hpp"
#include "api_error_helper.hpp"
#include "endpoints.hpp"

namespace infrastructure::persistence
{

void AppInfoAccess::getNewestAppVersion()
{
    auto request = createRequest(data::latestAppVersionEndpoint);
    auto reply = m_networkAccessManager.get(request);


    // Handle authentication result and release the reply's memory
    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply,
                                                      "Getting latest version");

                    emit newestAppVersionReceived("-");
                    reply->deleteLater();
                    return;
                }

                emit newestAppVersionReceived(reply->readAll());
                reply->deleteLater();
            });
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