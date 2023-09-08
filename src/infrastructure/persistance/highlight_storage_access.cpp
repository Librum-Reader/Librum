#include "highlight_storage_access.hpp"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include "api_error_helper.hpp"
#include "endpoints.hpp"

namespace infrastructure::persistence
{

HighlightStorageAccess::HighlightStorageAccess()
{
}

void HighlightStorageAccess::createHighlight(
    const QString& authToken, const QString& bookUuid,
    const adapters::dtos::HighlightDto& highlight)
{
    auto request = createRequest(data::authenticationEndpoint, authToken);

    QJsonObject jsonObject;

    QJsonDocument jsonDocument { jsonObject };
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    //    auto reply = m_networkAccessManager.post(request, data);

    // Handle authentication result and release the reply's memory
    //    connect(reply, &QNetworkReply::finished, this,
    //            [this, reply]()
    //            {
    //                if(api_error_helper::apiRequestFailed(reply, 200))
    //                {
    //                    auto errorCode = api_error_helper::logErrorMessage(
    //                        reply, "Authentication");

    //                    reply->deleteLater();
    //                    return;
    //                }

    //                reply->deleteLater();
    //            });
}

void HighlightStorageAccess::removeHighlight(const QString& authToken,
                                             const QString& bookUuid,
                                             const QString& highlightUuid)
{
}

QNetworkRequest HighlightStorageAccess::createRequest(const QUrl& url,
                                                      const QString& authToken)
{
    QNetworkRequest result { url };
    result.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    result.setRawHeader(QByteArray("Authorization"),
                        "Bearer " + authToken.toUtf8());

    QSslConfiguration sslConfiguration = result.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    result.setSslConfiguration(sslConfiguration);

    return result;
}

}  // namespace infrastructure::persistence