#include "book_storage_access.hpp"
#include <QDebug>
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

    m_bookCreationReply.reset(m_networkAccessManager.post(request, data));

    int expectedStatusCode = 201;
    linkRequestToErrorHandling(m_bookCreationReply.get(), expectedStatusCode);


    connect(m_bookCreationReply.get(), &QNetworkReply::finished,
            [this, expectedStatusCode, jsonBook, authToken]()
            {
                auto statusCode =
                    m_bookCreationReply
                        ->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                        .toInt();


                if(m_bookCreationReply->error() == QNetworkReply::NoError ||
                   expectedStatusCode == statusCode)
                {
                    uploadBookBinaryData(jsonBook["guid"].toString(),
                                         jsonBook["filePath"].toString(),
                                         authToken);
                }
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
    Q_UNUSED(authToken);
    Q_UNUSED(uuid);
}

void BookStorageAccess::proccessBookCreationResult()
{
    int expectedStatusCode = 201;
    auto replyStatus =
        validateServerReply(expectedStatusCode, m_bookCreationReply.get());
    if(!replyStatus.success)
    {
        emit creatingBookFinished(false, replyStatus.errorMessage);
        return;
    }

    emit creatingBookFinished(true, "");
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

void BookStorageAccess::uploadBookBinaryData(const QString& uuid,
                                             const QString& filePath,
                                             const QString& authToken)
{
    // Open file
    QFile* file = new QFile(QUrl(filePath).path());
    if(!file->open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open test file!";
        return;
    }

    // Build multi part
    m_multiPart.reset(new QHttpMultiPart(QHttpMultiPart::FormDataType));
    file->setParent(m_multiPart.get());
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader,
                       QVariant("application/octet-stream"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"file\"; filename=\"" +
                                file->fileName() + "\""));
    filePart.setBodyDevice(file);

    m_multiPart->append(filePart);

    auto testRequest =
        createRequest(data::bookBinaryDataEndpoint + "/" + uuid, authToken);
    testRequest.setHeader(QNetworkRequest::ContentTypeHeader, QByteArray());
    testRequest.setHeader(QNetworkRequest::ContentDispositionHeader,
                          QVariant("form-data; name=\"file\"; filename=\"" +
                                   file->fileName() + "\""));


    // Send request
    auto reply = m_networkAccessManager.post(testRequest, m_multiPart.get());
    m_testReply.reset(reply);

    linkRequestToErrorHandling(m_testReply.get(), 201);
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