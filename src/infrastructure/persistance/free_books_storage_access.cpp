#include "free_books_storage_access.hpp"
#include <QNetworkReply>
#include "api_error_helper.hpp"
#include "endpoints.hpp"

namespace infrastructure::persistence
{

void FreeBooksStorageAccess::getBooksMetadata()
{
    auto request = createRequest(data::getFreeBooksMetadata);

    auto getBooksMetadataReply = m_networkAccessManager.get(request);

    connect(getBooksMetadataReply, &QNetworkReply::finished, this,
            [this]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(
                        reply, "Getting free books metadata");

                    reply->deleteLater();
                    return;
                }

                emit gettingBooksMetadataFinished(reply->readAll());

                reply->deleteLater();
            });
}

QNetworkRequest FreeBooksStorageAccess::createRequest(const QUrl& url)
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
