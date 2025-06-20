#include "ai_tools_access.hpp"
#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QRegularExpression>
#include "api_error_helper.hpp"
#include "endpoints.hpp"

namespace infrastructure::persistence
{

AiToolsAccess::AiToolsAccess()
{
    QSettings appSettings;
    m_serverHost = appSettings.value("serverHost").toString();
}

void AiToolsAccess::getExplanation(const QString& authToken,
                                   const QString& query, const QString& mode)
{
    auto request =
        createRequest(m_serverHost + data::aiCompletionEndpoint, authToken);
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

                emit explanationWordReceived(word);
            });

    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    auto errorCode = api_error_helper::logErrorMessage(
                        reply, "Ai explanation");

                    emit explanationErrorOccured(errorCode);
                }

                reply->deleteLater();
            });
}

void AiToolsAccess::getTranslation(const QString& authToken,
                                   const QString& text,
                                   const QString& sourceLang,
                                   const QString& targetLang)
{
    auto request =
        createRequest(m_serverHost + data::aiTranslationEndpoint, authToken);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                         QNetworkRequest::AlwaysNetwork);

    QJsonObject body;
    body.insert("text", text);
    body.insert("sourcelang", sourceLang);
    body.insert("targetlang", targetLang);
    QJsonDocument jsonDocument { body };
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    auto reply = m_networkAccessManager.post(request, data);
    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    auto errorCode =
                        api_error_helper::logErrorMessage(reply, "Translation");

                    emit translationErrorOccured(errorCode);
                    reply->deleteLater();
                    return;
                }

                emit translationReceived(reply->readAll());
                reply->deleteLater();
            });
}

QNetworkRequest AiToolsAccess::createRequest(QUrl url, QString authToken) const
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
