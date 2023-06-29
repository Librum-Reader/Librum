#include "free_books_storage_access.hpp"
#include <QNetworkReply>
#include "api_error_helper.hpp"
#include "endpoints.hpp"

namespace infrastructure::persistence
{

void FreeBooksStorageAccess::getBooksMetadata()
{
    auto request = createRequest(data::getFreeBooksMetadata);

    auto reply = m_networkAccessManager.get(request);

    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
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

void FreeBooksStorageAccess::getCoverForBook(int bookId,
                                             const QString& coverUrl)
{
    auto request = createRequest(coverUrl);

    auto reply = m_networkAccessManager.get(request);

    connect(reply, &QNetworkReply::finished, this,
            [this, bookId, reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(
                        reply, "Getting free book's cover");

                    reply->deleteLater();
                    return;
                }

                emit gettingBookCoverFinished(bookId, reply->readAll());

                reply->deleteLater();
            });
}

void FreeBooksStorageAccess::getBookMedia(const QString& url)
{
    auto request = createRequest(url);

    auto reply = m_networkAccessManager.get(request);

    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(
                        reply, "Getting free book's media");

                    reply->deleteLater();
                    return;
                }

                emit gettingBookMediaFinished(reply->readAll());

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
