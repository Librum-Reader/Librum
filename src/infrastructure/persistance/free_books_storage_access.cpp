#include "free_books_storage_access.hpp"
#include <QNetworkReply>
#include "api_error_helper.hpp"
#include "endpoints.hpp"

namespace infrastructure::persistence
{

void FreeBooksStorageAccess::fetchFirstBooksMetadataPageWithFilter(
    const QString& authorsAndTitle)
{
    auto request = createGetBooksMetadataRequest(authorsAndTitle);

    auto reply = m_networkAccessManager.get(request);

    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
                auto success = !api_error_helper::apiRequestFailed(reply, 200);
                emit fetchingFirstMetadataPageSuccessful(success);

                if(!success)
                {
                    api_error_helper::logErrorMessage(
                        reply, "Fetching first free books metadata page");

                    reply->deleteLater();
                    return;
                }

                emit fetchingBooksMetaDataFinished(reply->readAll());
                reply->deleteLater();
            });
}

void FreeBooksStorageAccess::fetchBooksMetadataPage(const QString& url)
{
    auto request = createRequest(url);

    auto reply = m_networkAccessManager.get(request);

    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(
                        reply, "Fetching free books metadata page");

                    reply->deleteLater();
                    return;
                }

                emit fetchingBooksMetaDataFinished(reply->readAll());
                reply->deleteLater();
            });
}

void FreeBooksStorageAccess::getBookCover(int id, const QString& url)
{
    auto request = createRequest(url);

    auto reply = m_networkAccessManager.get(request);

    connect(reply, &QNetworkReply::finished, this,
            [this, id, reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(
                        reply, "Getting free book's cover");

                    reply->deleteLater();
                    return;
                }

                emit gettingBookCoverFinished(id, reply->readAll());
                reply->deleteLater();
            });
}

void FreeBooksStorageAccess::getBookMedia(const int id, const QUuid& uuid,
                                          const QString& url)
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

    connect(reply, &QNetworkReply::downloadProgress, this,
            [this, id](qint64 bytesReceived, qint64 bytesTotal)
            {
                emit gettingBookMediaProgressChanged(id, bytesReceived,
                                                     bytesTotal);
            });
}

QNetworkRequest FreeBooksStorageAccess::createGetBooksMetadataRequest(
    const QString& authorsAndTitle)
{
    if(!authorsAndTitle.isEmpty())
    {
        QString formattedAuthorsAndTitle = authorsAndTitle;
        formattedAuthorsAndTitle.replace(" ", m_whitespaceCode);

        return createRequest(data::getFreeBooksMetadataEndpoint +
                             "?search=" + formattedAuthorsAndTitle);
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
