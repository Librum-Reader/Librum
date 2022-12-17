#include "book_storage_access.hpp"
#include <QDebug>
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

    connect(m_bookCreationReply.get(), &QNetworkReply::finished, this,
            &BookStorageAccess::proccessBookCreationResult);
}

void BookStorageAccess::deleteBook(const QString& authToken, const QUuid& uuid)
{
    auto request = createRequest(data::bookDeletionEndpoint, authToken);

    QJsonArray bookArray;
    bookArray.append(QJsonValue::fromVariant(uuid));

    QJsonDocument jsonDocument(bookArray);
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    m_networkAccessManager.sendCustomRequest(request, "DELETE", data);
}

void BookStorageAccess::updateBook(const QString& authToken,
                                   const QJsonObject& jsonBook)
{
    QString endpoint =
        data::bookUpdateEndpoint + "/" + jsonBook["uuid"].toString();
    auto request = createRequest(endpoint, authToken);

    QJsonDocument jsonDocument(jsonBook);
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    auto reply = m_networkAccessManager.sendCustomRequest(request, "PUT", data);
    m_bookUpdatingReply.reset(reply);

    connect(m_bookUpdatingReply.get(), &QNetworkReply::finished, this,
            [this]()
            {
                checkForErrors(200, m_bookUpdatingReply.get());
            });
}

void BookStorageAccess::getBooksMetaData(const QString& authToken)
{
    auto request = createRequest(data::getBooksMetadataEndpoint, authToken);
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
    if(checkForErrors(expectedStatusCode, m_bookCreationReply.get()))
    {
        QString reason = m_bookCreationReply->readAll();
        emit creatingBookFinished(false, reason);
        return;
    }

    emit creatingBookFinished(true, "");
}

void BookStorageAccess::proccessGettingBooksMetaDataResult()
{
    // Error handling
    int expectedStatusCode = 200;
    if(checkForErrors(expectedStatusCode, m_gettingBooksMetaDataReply.get()))
    {
        std::vector<QJsonObject> empty;
        emit gettingBooksMetaDataFinished(empty);
        return;
    }

    // Parsing
    auto jsonReply =
        QJsonDocument::fromJson(m_gettingBooksMetaDataReply->readAll());
    auto jsonBooks = jsonReply.array();

    std::vector<QJsonObject> books;
    for(const auto& jsonBook : jsonBooks)
    {
        books.emplace_back(jsonBook.toObject());
    }

    // Sending result
    emit gettingBooksMetaDataFinished(books);
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

bool BookStorageAccess::checkForErrors(int expectedStatusCode,
                                       QNetworkReply* reply)
{
    if(reply->error() != QNetworkReply::NoError)
        qDebug() << "there was an error! " << reply->errorString();

    int statusCode =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statusCode != expectedStatusCode)
    {
        qDebug() << "there was an error! " << reply->readAll();
        return true;
    }

    return false;
}

}  // namespace infrastructure::persistence