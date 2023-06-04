#include "book_storage_access.hpp"
#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSslConfiguration>
#include "api_error_helper.hpp"
#include "endpoints.hpp"

namespace infrastructure::persistence
{

void BookStorageAccess::createBook(const QString& authToken,
                                   const QJsonObject& jsonBook)
{
    auto request = createRequest(data::bookCreationEndpoint, authToken);

    QJsonDocument jsonDocument(jsonBook);
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    auto bookCreationReply = m_networkAccessManager.post(request, data);


    // The book is created in separate steps. First of all an enty for the book
    // is created in the SQL Database, if that succeeds the book's data (the
    // actual binary file) and its cover are uploaded to the server.
    connect(
        bookCreationReply, &QNetworkReply::finished, this,
        [this, jsonBook, authToken]()
        {
            auto reply = qobject_cast<QNetworkReply*>(sender());
            if(api_error_helper::apiRequestFailed(reply, 201))
            {
                api_error_helper::logErrorMessage(reply, "Creating book");

                // Manually handle "Storage limit exceeded" error
                if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                       .toInt() == 426)
                {
                    emit storageLimitExceeded();
                }

                reply->deleteLater();
                return;
            }

            // Send book's actual binary file to the server
            uploadBookMedia(jsonBook["guid"].toString(),
                            jsonBook["filePath"].toString(), authToken);

            // Send the book's cover to the server
            uploadBookCover(authToken, jsonBook["guid"].toString(),
                            jsonBook["coverPath"].toString());

            // Make sure to release the reply's memory
            reply->deleteLater();
        });
}

void BookStorageAccess::deleteBook(const QString& authToken, const QUuid& uuid)
{
    auto request = createRequest(data::bookDeletionEndpoint, authToken);

    QJsonArray bookArray;
    bookArray.append(QJsonValue::fromVariant(uuid));

    QJsonDocument jsonDocument(bookArray);
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    auto deletionReply =
        m_networkAccessManager.sendCustomRequest(request, "DELETE", data);


    // Validate and release the reply's memory
    connect(deletionReply, &QNetworkReply::finished, this,
            [this]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 204))
                {
                    api_error_helper::logErrorMessage(reply, "Deleting");
                }

                reply->deleteLater();
            });
}

void BookStorageAccess::updateBook(const QString& authToken,
                                   const QJsonObject& jsonBook)
{
    auto request = createRequest(data::bookUpdateEndpoint, authToken);

    QJsonDocument jsonDocument(jsonBook);
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    auto updateReply = m_networkAccessManager.put(request, data);


    // Validate and release the reply's memory
    connect(updateReply, &QNetworkReply::finished, this,
            [this]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply, "Updating book");
                }

                reply->deleteLater();
            });
}

void BookStorageAccess::uploadBookCover(const QString& authToken,
                                        const QUuid& uuid, const QString& path)
{
    auto bookCover = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QString stringUuid = uuid.toString(QUuid::WithoutBraces);

    QFile* file = new QFile(QUrl(path).path());
    if(!file->open(QIODevice::ReadOnly))
    {
        qDebug() << QString("Could not open cover for book with uuid: %1")
                        .arg(stringUuid);

        bookCover->deleteLater();
        return;
    }

    // Make sure the file is released when the request finished.
    file->setParent(bookCover);

    QHttpPart imagePart;
    imagePart.setBodyDevice(file);
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader,
                        QVariant("image/png"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant("form-data; name=\"image\"; filename=\"" +
                                 file->fileName() + "\""));


    bookCover->append(imagePart);


    QUrl endpoint = data::changeBookCoverEndpoint + "/" + stringUuid;
    auto request = createRequest(endpoint, authToken);

    // Reset the ContentTypeHeader since it will be set by the multipart
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArray());

    auto coverUploadreply = m_networkAccessManager.post(request, bookCover);

    // Make sure to free the data used for uploading the cover.
    connect(coverUploadreply, &QNetworkReply::finished, this,
            [this, bookCover]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply,
                                                      "Uploading book cover");

                    reply->deleteLater();
                    return;
                }

                reply->deleteLater();
                bookCover->deleteLater();
            });
}

void BookStorageAccess::deleteBookCover(const QString& authToken,
                                        const QUuid& uuid)
{
    QUrl endpoint = data::deleteBookCoverEndpoint + "/" +
                    uuid.toString(QUuid::WithoutBraces);
    auto request = createRequest(endpoint, authToken);

    auto coverDeletionReply =
        m_networkAccessManager.sendCustomRequest(request, "DELETE");

    // Make sure to release the reply's memory
    connect(coverDeletionReply, &QNetworkReply::finished, this,
            [this]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply,
                                                      "Deleting book cover");

                    reply->deleteLater();
                    return;
                }

                reply->deleteLater();
            });
}

void BookStorageAccess::getBooksMetaData(const QString& authToken)
{
    auto request = createRequest(data::booksMetadataGetEndpoint, authToken);
    auto reply = m_networkAccessManager.get(request);

    connect(reply, &QNetworkReply::finished, this,
            &BookStorageAccess::processGettingBooksMetaDataResult);
}

void BookStorageAccess::downloadCoverForBook(const QString& authToken,
                                             const QUuid& uuid)
{
    QString uuidString = uuid.toString(QUuid::WithoutBraces);
    QString endpoint = data::getBookCoverEndpoint + "/" + uuidString;
    auto request = createRequest(endpoint, authToken);
    auto coverDownloadreply = m_networkAccessManager.get(request);


    // Make sure to release the reply's memory
    connect(coverDownloadreply, &QNetworkReply::finished, this,
            [this]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply,
                                                      "Downloading book cover");

                    reply->deleteLater();
                    return;
                }


                // The book's uuid (server calls it: "guid") is sent as a header
                QString bookGuid = reply->rawHeader("Guid");

                emit downloadingBookCoverFinished(reply->readAll(), bookGuid);
                reply->deleteLater();
            });
}

void BookStorageAccess::downloadBookMedia(const QString& authToken,
                                          const QUuid& uuid)
{
    auto endpoint = data::downloadBookDataEndpoint + "/" +
                    uuid.toString(QUuid::WithoutBraces);
    auto request = createRequest(endpoint, authToken);
    auto bookDownloadReply = m_networkAccessManager.get(request);


    // Handle the received books and release the reply's memory
    connect(bookDownloadReply, &QNetworkReply::finished, this,
            [this]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply,
                                                      "Downloading book media");

                    reply->deleteLater();
                    return;
                }


                // The book's uuid (server calls it: "guid") is sent as a header
                QString bookGuid = reply->rawHeader("Guid");
                QString bookFormat = reply->rawHeader("Format");

                emit downloadingBookMediaFinished(reply->readAll(), bookGuid,
                                                  bookFormat);
                reply->deleteLater();
            });
}

void BookStorageAccess::processGettingBooksMetaDataResult()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    if(api_error_helper::apiRequestFailed(reply, 200))
    {
        api_error_helper::logErrorMessage(reply, "Getting books");

        std::vector<QJsonObject> empty;
        emit gettingBooksMetaDataFinished(empty);
        reply->deleteLater();
        return;
    }


    auto jsonReply = QJsonDocument::fromJson(reply->readAll());
    auto jsonBooks = jsonReply.array();

    std::vector<QJsonObject> books;
    for(const auto& jsonBook : jsonBooks)
    {
        books.emplace_back(jsonBook.toObject());
    }

    emit gettingBooksMetaDataFinished(books);
    reply->deleteLater();
}

void BookStorageAccess::uploadBookMedia(const QString& uuid,
                                        const QString& filePath,
                                        const QString& authToken)
{
    auto bookData = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    auto success = addFilePartToMultiPart(bookData, filePath);
    if(!success)
    {
        bookData->deleteLater();
        return;
    }


    QUrl endpoint = data::uploadBookDataEndpoint + "/" + uuid;
    auto request = createRequest(endpoint, authToken);

    // Reset the ContentTypeHeader since it will be set by the multipart
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArray());

    auto bookUploadReply = m_networkAccessManager.post(request, bookData);


    // Make sure to free the data used for uploading the book data.
    connect(bookUploadReply, &QNetworkReply::finished, this,
            [this, bookData]()
            {
                auto reply = qobject_cast<QNetworkReply*>(sender());
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply,
                                                      "Uploading book media");
                }

                reply->deleteLater();
                bookData->deleteLater();
            });
}

bool BookStorageAccess::addFilePartToMultiPart(QHttpMultiPart* bookData,
                                               const QUrl& path)
{
    QFile* file = new QFile(QUrl(path).path());
    if(!file->open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open book data file";
        return false;
    }

    // Make sure the file is released when the request finished.
    file->setParent(bookData);

    QHttpPart filePart;
    filePart.setBodyDevice(file);
    filePart.setHeader(QNetworkRequest::ContentTypeHeader,
                       QVariant("application/octet-stream"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"file\"; filename=\"" +
                                file->fileName() + "\""));


    bookData->append(filePart);
    return true;
}

QNetworkRequest BookStorageAccess::createRequest(const QUrl& url,
                                                 const QString& authToken)
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