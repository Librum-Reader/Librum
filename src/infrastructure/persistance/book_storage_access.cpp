#include "book_storage_access.hpp"
#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSslConfiguration>
#include "endpoints.hpp"

namespace infrastructure::persistence
{

void BookStorageAccess::createBook(const QString& authToken,
                                   const QJsonObject& jsonBook)
{
    auto request = createRequest(data::bookCreationEndpoint, authToken);

    QJsonDocument jsonDocument(jsonBook);
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    int expectedStatusCode = 201;
    auto reply = m_networkAccessManager.post(request, data);
    m_bookCreationReply.reset(reply);
    linkRequestToErrorHandling(m_bookCreationReply.get(), expectedStatusCode);


    // Upload book's binary data if creating the book succeeded
    connect(m_bookCreationReply.get(), &QNetworkReply::finished, this,
            [this, jsonBook, authToken, expectedStatusCode]()
            {
                auto statusCode = m_bookCreationReply->attribute(
                    QNetworkRequest::HttpStatusCodeAttribute);

                if(statusCode.toInt() != expectedStatusCode)
                    return;

                uploadBookData(jsonBook["guid"].toString(),
                               jsonBook["filePath"].toString(), authToken);
            });
}

void BookStorageAccess::deleteBook(const QString& authToken, const QUuid& uuid)
{
    auto request = createRequest(data::bookDeletionEndpoint, authToken);

    QJsonArray bookArray;
    bookArray.append(QJsonValue::fromVariant(uuid));

    QJsonDocument jsonDocument(bookArray);
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    auto reply =
        m_networkAccessManager.sendCustomRequest(request, "DELETE", data);
    m_bookDeletionReply.reset(reply);

    linkRequestToErrorHandling(m_bookDeletionReply.get(), 204);
}

void BookStorageAccess::updateBook(const QString& authToken,
                                   const QJsonObject& jsonBook)
{
    auto request = createRequest(data::bookUpdateEndpoint, authToken);

    QJsonDocument jsonDocument(jsonBook);
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    m_bookUpdateReply.reset(m_networkAccessManager.put(request, data));
    linkRequestToErrorHandling(m_bookUpdateReply.get(), 200);
}

void BookStorageAccess::changeBookCover(const QString& authToken,
                                        const QUuid& uuid, const QString& path)
{
    m_bookCoverMultiPart.reset(
        new QHttpMultiPart(QHttpMultiPart::FormDataType));
    QString stringUuid = uuid.toString(QUuid::WithoutBraces);

    QFile* file = new QFile(QUrl(path).path());
    if(!file->open(QIODevice::ReadOnly))
    {
        qDebug() << QString("Could not open cover for book with uuid: %1")
                        .arg(stringUuid);
        return;
    }

    // Make sure the file is released when the request finished.
    file->setParent(m_bookCoverMultiPart.get());

    QHttpPart imagePart;
    imagePart.setBodyDevice(file);
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader,
                        QVariant("image/png"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant("form-data; name=\"image\"; filename=\"" +
                                 file->fileName() + "\""));


    m_bookCoverMultiPart->append(imagePart);


    QUrl endpoint = data::changeBookCoverEndpoint + "/" + stringUuid;
    auto request = createRequest(endpoint, authToken);

    // Reset the ContentTypeHeader since it will be set by the multipart
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArray());

    auto r = m_networkAccessManager.post(request, m_bookCoverMultiPart.get());
    m_bookCoverUploadReply.reset(r);
    linkRequestToErrorHandling(m_bookCoverUploadReply.get(), 201);


    // Make sure to free the data used for uploading the cover.
    // The m_bookCoverUploadReply is set as the parent for e.g. the file.
    connect(m_bookCoverUploadReply.get(), &QNetworkReply::finished, this,
            [this]()
            {
                m_bookDataMultiPart.reset();
            });
}

void BookStorageAccess::deleteBookCover(const QString& authToken,
                                        const QUuid& uuid)
{
    QUrl endpoint = data::deleteBookCoverEndpoint + "/" +
                    uuid.toString(QUuid::WithoutBraces);
    auto request = createRequest(endpoint, authToken);

    auto reply = m_networkAccessManager.sendCustomRequest(request, "DELETE");
    m_bookCoverDeletionReply.reset(reply);
    linkRequestToErrorHandling(m_bookCoverDeletionReply.get(), 204);
}

void BookStorageAccess::getBooksMetaData(const QString& authToken)
{
    auto request = createRequest(data::booksMetadataGetEndpoint, authToken);
    m_gettingBooksMetaDataReply.reset(m_networkAccessManager.get(request));

    connect(m_gettingBooksMetaDataReply.get(), &QNetworkReply::finished, this,
            &BookStorageAccess::proccessGettingBooksMetaDataResult);
}

void BookStorageAccess::downloadBook(const QString& authToken,
                                     const QUuid& uuid)
{
    auto endpoint = data::downloadBookDataEndpoint + "/" +
                    uuid.toString(QUuid::WithoutBraces);
    auto request = createRequest(endpoint, authToken);

    auto reply = m_networkAccessManager.get(request);
    m_bookDataDownloadReply.reset(reply);

    connect(m_bookDataDownloadReply.get(), &QNetworkReply::finished, this,
            &BookStorageAccess::proccessDownloadBookResult);

    linkRequestToErrorHandling(m_bookDataDownloadReply.get(), 200);
}

void BookStorageAccess::proccessGettingBooksMetaDataResult()
{
    auto replyStatus =
        validateServerReply(200, m_gettingBooksMetaDataReply.get());
    if(!replyStatus.success)
    {
        std::vector<QJsonObject> empty;
        emit gettingBooksMetaDataFinished(empty);
        return;
    }


    auto jsonReply =
        QJsonDocument::fromJson(m_gettingBooksMetaDataReply->readAll());
    auto jsonBooks = jsonReply.array();

    std::vector<QJsonObject> books;
    for(const auto& jsonBook : jsonBooks)
    {
        books.emplace_back(jsonBook.toObject());
    }

    emit gettingBooksMetaDataFinished(books);
}

void BookStorageAccess::proccessDownloadBookResult()
{
    // The book uuid (or as the server calls it "guid") is send as a header
    QString bookGuid = m_bookDataDownloadReply->rawHeader("Guid");

    emit downloadingBookFinished(m_bookDataDownloadReply->readAll(), bookGuid);
}

void BookStorageAccess::uploadBookData(const QString& uuid,
                                       const QString& filePath,
                                       const QString& authToken)
{
    m_bookDataMultiPart.reset(new QHttpMultiPart(QHttpMultiPart::FormDataType));
    setupDataMultiPartWithFile(filePath);


    QUrl endpoint = data::uploadBookDataEndpoint + "/" + uuid;
    auto request = createRequest(endpoint, authToken);

    // Reset the ContentTypeHeader since it will be set by the multipart
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArray());

    auto r = m_networkAccessManager.post(request, m_bookDataMultiPart.get());
    m_bookDataUploadReply.reset(r);
    linkRequestToErrorHandling(m_bookDataUploadReply.get(), 201);


    // Make sure to free the data used for uploading the book binary data.
    // The m_bookDataUploadReply is set as the parent for e.g. the file.
    connect(m_bookDataUploadReply.get(), &QNetworkReply::finished, this,
            [this]()
            {
                m_bookDataMultiPart.reset();
            });
}

void BookStorageAccess::setupDataMultiPartWithFile(const QUrl& path)
{
    QFile* file = new QFile(QUrl(path).path());
    if(!file->open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open book data file";
        return;
    }

    // Make sure the file is released when the request finished.
    file->setParent(m_bookDataMultiPart.get());

    QHttpPart filePart;
    filePart.setBodyDevice(file);
    filePart.setHeader(QNetworkRequest::ContentTypeHeader,
                       QVariant("application/octet-stream"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"file\"; filename=\"" +
                                file->fileName() + "\""));


    m_bookDataMultiPart->append(filePart);
}

QNetworkRequest BookStorageAccess::createRequest(const QUrl& url,
                                                 const QString& authToken)
{
    QNetworkRequest result { url };
    result.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    result.setRawHeader("X-Version", "1.0");
    result.setRawHeader(QByteArray("Authorization"),
                        "Bearer " + authToken.toUtf8());

    QSslConfiguration sslConfiguration = result.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    result.setSslConfiguration(sslConfiguration);

    return result;
}

void BookStorageAccess::linkRequestToErrorHandling(QNetworkReply* reply,
                                                   int expectedStatusCode)
{
    connect(reply, &QNetworkReply::finished, this,
            [this, expectedStatusCode, reply]()
            {
                validateServerReply(expectedStatusCode, reply);
            });
}

ServerReplyStatus BookStorageAccess::validateServerReply(int expectedStatusCode,
                                                         QNetworkReply* reply)
{
    auto statusCode =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();


    if(reply->error() != QNetworkReply::NoError ||
       expectedStatusCode != statusCode)
    {
        auto errorMessage = reply->readAll();
        qWarning() << QString("Authentication error: %1 \nServer replied: %2")
                          .arg(reply->errorString(), errorMessage);

        return ServerReplyStatus {
            .success = false,
            .errorMessage = errorMessage,
        };
    }

    return ServerReplyStatus { .success = true };
}

}  // namespace infrastructure::persistence