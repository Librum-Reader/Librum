#include "dictionary_access.hpp"
#include <QNetworkReply>
#include "api_error_helper.hpp"
#include "endpoints.hpp"
#include "qjsonobject.h"

namespace infrastructure::persistence
{

void DictionaryAccess::getDefinitionForWord(const QString& word)
{
    auto url = data::dictionaryDefinitionEndpoint + "/" + word;
    auto request = createRequest(url);
    auto reply = m_networkAccessManager.get(request);


    // Handle authentication result and release the reply's memory
    connect(reply, &QNetworkReply::finished, this,
            [reply]()
            {
                QJsonObject jsonReply =
                    QJsonDocument::fromJson(reply->readAll()).object();

                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply,
                                                      "Getting definition");

                    reply->deleteLater();
                    return;
                }

                reply->deleteLater();
            });
}

QNetworkRequest DictionaryAccess::createRequest(QUrl url)
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