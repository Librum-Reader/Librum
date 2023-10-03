#include "ai_explanation_access.hpp"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include "api_error_helper.hpp"
#include "endpoints.hpp"

namespace infrastructure::persistence
{

void AiExplanationAccess::getExplanation(const QString& authToken,
                                         const QString& query,
                                         const QString& mode)
{
    auto request = createRequest(data::aiCompletionEndpoint, authToken);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                         QNetworkRequest::AlwaysNetwork);

    QJsonObject body;
    body.insert("text", query);
    body.insert("mode", mode);
    QJsonDocument jsonDocument { body };
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    auto reply = m_networkAccessManager.post(request, data);

    connect(
        reply, &QNetworkReply::readyRead, this,
        [reply]()
        {
            qDebug()
                << QString(reply->readAll()).simplified().replace("data: ", "");
        });

    //    // Handle api result and release the reply's memory
    //    connect(reply, &QNetworkReply::finished, this,
    //            [this, reply]()
    //            {
    //                if(api_error_helper::apiRequestFailed(reply, 200))
    //                {
    //                    api_error_helper::logErrorMessage(reply,
    //                                                      "Getting ai
    //                                                      explanation");

    //                    emit explanationReceived("");
    //                    reply->deleteLater();
    //                    return;
    //                }

    //                emit explanationReceived(reply->readAll());
    //                reply->deleteLater();
    //            });
}

QNetworkRequest AiExplanationAccess::createRequest(QUrl url,
                                                   QString authToken) const
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