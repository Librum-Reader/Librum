#include "free_books_storage_access.hpp"
#include <QNetworkReply>
#include "api_error_helper.hpp"
#include "endpoints.hpp"

namespace infrastructure::persistence
{

void FreeBooksStorageAccess::getBooksMetadata(const QString& author,
                                              const QString& title)
{
    auto request = createGetBooksMetadataRequest(author, title);

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

void FreeBooksStorageAccess::getBookMedia(const QString& url, const QUuid& uuid)
{
    auto request = createRequest(url);
    auto reply = m_networkAccessManager.get(request);

    connect(reply, &QNetworkReply::readyRead, this,
            [this, reply, uuid]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(
                        reply, "Getting free book's media");

                    reply->deleteLater();
                    return;
                }

                emit gettingBookMediaChunkReady(reply->readAll(), false, uuid,
                                                "epub");
            });

    connect(reply, &QNetworkReply::finished, this,
            [this, reply, uuid]()
            {
                emit gettingBookMediaChunkReady(QByteArray(), true, uuid,
                                                "epub");

                reply->deleteLater();
            });
}

QNetworkRequest FreeBooksStorageAccess::createGetBooksMetadataRequest(
    const QString& author, const QString& title)
{
    if(!author.isEmpty() || !title.isEmpty())
    {
        QString formattedAuthor = author;
        QString formattedTitle = title;
        formattedAuthor.replace(" ", m_whitespaceCode);
        formattedTitle.replace(" ", m_whitespaceCode);

        return createRequest(data::getFreeBooksMetadataEndpoint +
                             "?search=" + formattedAuthor + m_whitespaceCode +
                             formattedTitle);
    }

    return createRequest(data::getFreeBooksMetadataEndpoint + "/");
}

QNetworkRequest FreeBooksStorageAccess::createRequest(const QUrl& url)
{
    QNetworkRequest result { url };
    result.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    result.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);

    QSslConfiguration sslConfiguration = result.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    result.setSslConfiguration(sslConfiguration);

    return result;
}

}  // namespace infrastructure::persistence
