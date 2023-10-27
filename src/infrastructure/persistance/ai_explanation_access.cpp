#include "ai_explanation_access.hpp"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QRegularExpression>
#include "api_error_helper.hpp"
#include "endpoints.hpp"

namespace infrastructure::persistence
{

AiExplanationAccess::AiExplanationAccess()
{
    QSettings settings;
    domain = settings.value("serverHost").toString();
}

void AiExplanationAccess::getExplanation(const QString& authToken,
                                         const QString& query,
                                         const QString& mode)
{
    auto request =
        createRequest(domain + data::aiCompletionEndpoint, authToken);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                         QNetworkRequest::AlwaysNetwork);

    QJsonObject body;
    body.insert("text", query);
    body.insert("mode", mode);
    QJsonDocument jsonDocument { body };
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    auto reply = m_networkAccessManager.post(request, data);
    auto requestStartTime = QDateTime::currentDateTimeUtc();
    m_lastRequestStartTime = requestStartTime;

    connect(reply, &QNetworkReply::readyRead, this,
            [this, reply, requestStartTime]()
            {
                auto statusCode =
                    reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                        .toInt();
                if(statusCode != 200)
                    return;

                // Make sure to not process "old" requests. We can not abort the
                // SSE so the server will continue sending data, we just need to
                // ignore old requests.
                if(m_lastRequestStartTime != requestStartTime)
                    return;

                QString word = reply->readAll().replace("data: ", "");
                int i = 0;
                int k = 1;

                while(k < word.length())
                {
                    if(word[i] == '\n' && word[k] == '\n')
                    {
                        word.removeAt(k);
                        word.removeAt(i);
                    }

                    i += 1;
                    k += 1;
                };

                emit wordReceived(word);
            });

    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    auto errorCode = api_error_helper::logErrorMessage(
                        reply, "Ai explanation");

                    emit errorOccured(errorCode);
                }

                reply->deleteLater();
            });
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
