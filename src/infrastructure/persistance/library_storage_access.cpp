#include "library_storage_access.hpp"
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

LibraryStorageAccess::LibraryStorageAccess(){
	QSettings settings;
	domain=settings.value("serverHost").toString();
}

void LibraryStorageAccess::createBook(const QString& authToken,
                                      const QJsonObject& jsonBook)
{
   auto request = createRequest(domain + data::bookCreationEndpoint, authToken);

    QJsonDocument jsonDocument(jsonBook);
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    auto reply = m_networkAccessManager.post(request, data);


    // The book is created in separate steps. First of all an enty for the book
    // is created in the SQL Database, if that succeeds the book's data (the
    // actual binary file) and its cover are uploaded to the server.
    connect(
        reply, &QNetworkReply::finished, this,
        [this, reply, jsonBook, authToken]()
        {
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

            auto uuid = jsonBook["guid"].toString();
            uploadBookMedia(uuid, jsonBook["filePath"].toString(), authToken);
            uploadBookCover(authToken, QUuid(uuid),
                            jsonBook["coverPath"].toString());

            // Make sure to release the reply's memory
            reply->deleteLater();
        });
}

void LibraryStorageAccess::deleteBook(const QString& authToken,
                                      const QUuid& uuid)
{
    auto request = createRequest(domain + data::bookDeletionEndpoint, authToken);

    QJsonArray bookArray;
    bookArray.append(QJsonValue::fromVariant(uuid));

    QJsonDocument jsonDocument(bookArray);
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    auto reply =
        m_networkAccessManager.sendCustomRequest(request, "DELETE", data);


    // Validate and release the reply's memory
    connect(reply, &QNetworkReply::finished, this,
            [reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 204))
                {
                    api_error_helper::logErrorMessage(reply, "Deleting");
                }

                reply->deleteLater();
            });
}

void LibraryStorageAccess::updateBook(const QString& authToken,
                                      const QJsonObject& jsonBook)
{
    auto request = createRequest(domain + data::bookUpdateEndpoint, authToken);

    QJsonDocument jsonDocument(jsonBook);
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    auto reply = m_networkAccessManager.put(request, data);


    // Validate and release the reply's memory
    connect(reply, &QNetworkReply::finished, this,
            [reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply, "Updating book");
                }

                reply->deleteLater();
            });
}

void LibraryStorageAccess::uploadBookCover(const QString& authToken,
                                           const QUuid& uuid,
                                           const QString& path)
{
    auto bookCover = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QString stringUuid = uuid.toString(QUuid::WithoutBraces);

    QFile* file = new QFile(path);
    if(!file->open(QIODevice::ReadOnly))
    {
        qWarning() << QString("Could not open cover for book with uuid: %1")
                          .arg(stringUuid);

        bookCover->deleteLater();
        file->deleteLater();
        return;
    }

    // Make sure the file is released when the request finished.
    file->setParent(bookCover);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader,
                        QVariant("image/png"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant("form-data; name=\"image\"; filename=\"" +
                                 file->fileName() + "\""));
    imagePart.setBodyDevice(file);
    bookCover->append(imagePart);


   QUrl endpoint =domain +  data::changeBookCoverEndpoint + "/" + stringUuid;
    auto request = createRequest(endpoint, authToken);

    // Reset the ContentTypeHeader since it will be set by the multipart
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant());

    auto reply = m_networkAccessManager.post(request, bookCover);

    // Make sure to free the data used for uploading the cover.
    connect(reply, &QNetworkReply::finished, this,
            [reply, bookCover]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply,
                                                      "Uploading book cover");

                    reply->deleteLater();
                    bookCover->deleteLater();
                    return;
                }

                reply->deleteLater();
                bookCover->deleteLater();
            });
}

void LibraryStorageAccess::deleteBookCover(const QString& authToken,
                                           const QUuid& uuid)
{
    QUrl endpoint =domain +  data::deleteBookCoverEndpoint + "/" +
                    uuid.toString(QUuid::WithoutBraces);
    auto request = createRequest(endpoint, authToken);

    auto reply = m_networkAccessManager.sendCustomRequest(request, "DELETE");

    // Make sure to release the reply's memory
    connect(reply, &QNetworkReply::finished, this,
            [reply]()
            {
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

void LibraryStorageAccess::getBooksMetaData(const QString& authToken)
{
    auto request = createRequest(domain + data::booksMetadataGetEndpoint, authToken);
    auto reply = m_networkAccessManager.get(request);

    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply, "Getting books");
                    reply->deleteLater();
                    return;
                }

                processGettingBooksMetaDataResult(reply);
                reply->deleteLater();
            });
}

void LibraryStorageAccess::downloadCoverForBook(const QString& authToken,
                                                const QUuid& uuid)
{
    QString uuidString = uuid.toString(QUuid::WithoutBraces);
   QString endpoint =domain +  data::getBookCoverEndpoint + "/" + uuidString;
    auto request = createRequest(endpoint, authToken);
    auto reply = m_networkAccessManager.get(request);


    // Make sure to release the reply's memory
    connect(reply, &QNetworkReply::finished, this,
            [this, reply, uuid]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply,
                                                      "Downloading book cover");

                    reply->deleteLater();
                    return;
                }

                emit downloadingBookCoverFinished(reply->readAll(), uuid);
                reply->deleteLater();
            });
}

void LibraryStorageAccess::downloadBookMedia(const QString& authToken,
                                             const QUuid& uuid)
{
    auto endpoint =domain +  data::downloadBookDataEndpoint + "/" +
                    uuid.toString(QUuid::WithoutBraces);
    auto request = createRequest(endpoint, authToken);
    auto reply = m_networkAccessManager.get(request);

    connect(reply, &QNetworkReply::readyRead, this,
            [this, reply, uuid]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply,
                                                      "Downloading book media");

                    reply->deleteLater();
                    return;
                }

                // The server sends the book format in a header
                QString bookFormat = reply->rawHeader("Format");
                emit downloadingBookMediaChunkReady(reply->readAll(), false,
                                                    uuid, bookFormat);
            });

    connect(reply, &QNetworkReply::finished, this,
            [this, reply, uuid]()
            {
                QString bookFormat = reply->rawHeader("Format");
                emit downloadingBookMediaChunkReady(QByteArray(), true, uuid,
                                                    bookFormat);

                reply->deleteLater();
            });

    connect(reply, &QNetworkReply::downloadProgress, this,
            [this, uuid](qint64 bytesReceived, qint64 bytesTotal)
            {
                emit downloadingBookMediaProgressChanged(uuid, bytesReceived,
                                                         bytesTotal);
            });
}

void LibraryStorageAccess::processGettingBooksMetaDataResult(
    QNetworkReply* reply)
{
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

void LibraryStorageAccess::uploadBookMedia(const QString& uuid,
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


    QUrl endpoint =domain +  data::uploadBookDataEndpoint + "/" + uuid;
    auto request = createRequest(endpoint, authToken);

    // Reset the ContentTypeHeader since it will be set by the multipart
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant());

    auto reply = m_networkAccessManager.post(request, bookData);


    // Make sure to free the data used for uploading the book data.
    connect(reply, &QNetworkReply::finished, this,
            [this, reply, bookData, uuid]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(reply,
                                                      "Uploading book media");
                    reply->deleteLater();
                    bookData->deleteLater();
                    return;
                }

                emit bookUploadSucceeded(QUuid(uuid));
                reply->deleteLater();
                bookData->deleteLater();
            });
}

bool LibraryStorageAccess::addFilePartToMultiPart(QHttpMultiPart* bookData,
                                                  const QString& path)
{
    QFile* file = new QFile(path);
    if(!file->open(QIODevice::ReadOnly))
    {
        qWarning() << "Could not open book data file";
        file->deleteLater();
        return false;
    }

    // Make sure the file is released when the request finished.
    file->setParent(bookData);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader,
                       QVariant("application/octet-stream"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"file\"; filename=\"" +
                                file->fileName() + "\""));
    filePart.setBodyDevice(file);


    bookData->append(filePart);
    return true;
}

QNetworkRequest LibraryStorageAccess::createRequest(const QUrl& url,
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
