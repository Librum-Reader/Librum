#include "book_storage_access.hpp"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSslConfiguration>


using namespace adapters::dtos;


namespace infrastructure::persistence
{

BookStorageAccess::BookStorageAccess()
    : m_bookCreationEndpoint("https://localhost:7084/api/book/create")
{
}


void BookStorageAccess::createBook(const QString& authToken,
                                   const BookDto& bookDto)
{
    auto request = createRequest(m_bookCreationEndpoint, authToken);
    
    QJsonObject jsonObject;
    jsonObject["guid"] = bookDto.uuid;
    jsonObject["title"] = bookDto.title;
    jsonObject["creator"] = bookDto.creator;
    jsonObject["creationDate"] = bookDto.creationDate;
    jsonObject["format"] = bookDto.format;
    jsonObject["language"] = bookDto.language;
    jsonObject["documentSize"] = bookDto.documentSize;
    jsonObject["pagesSize"] = bookDto.pagesSize;
    jsonObject["pageCount"] = bookDto.pageCount;
    jsonObject["currentPage"] = bookDto.currentPage;
    jsonObject["addedToLibrary"] = bookDto.addedToLibrary;
    jsonObject["lastOpened"] = bookDto.lastOpened;
    jsonObject["cover"] = "someCover";
    
    QJsonDocument jsonDocument{jsonObject};
    QByteArray data = jsonDocument.toJson();
    
    
    m_reply.reset(m_networkAccessManager.post(request, data));
    
    connect(m_reply.get(), &QNetworkReply::finished,
            this, &BookStorageAccess::proccessBookCreationResult);
}

void BookStorageAccess::deleteBook(const QString& authToken,
                                   const QUuid& uuid)
{
    Q_UNUSED(authToken);
    Q_UNUSED(uuid);
}

void BookStorageAccess::updateBook(const QString& authToken,
                                   const BookDto& bookDto)
{
    Q_UNUSED(authToken);
    Q_UNUSED(bookDto);
}

std::vector<BookDto> BookStorageAccess::getBooksMetaData(const QString& authToken)
{
    Q_UNUSED(authToken);
    return {};
}

void BookStorageAccess::downloadBook(const QString& authToken, const QUuid& uuid)
{
    Q_UNUSED(authToken);
    Q_UNUSED(uuid);
}


void BookStorageAccess::proccessBookCreationResult()
{
    int expectedStatusCode = 201;
    if(checkForErrors(expectedStatusCode))
    {
        QString reason = m_reply->readAll();
        emit creatingBookFinished(false, reason);
        return;
    }
    
    emit creatingBookFinished(true, "");
}


QNetworkRequest BookStorageAccess::createRequest(const QUrl& url, 
                                                 const QString& authToken)
{
    QNetworkRequest result{ url };
    result.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    result.setRawHeader("X-Version", "1.0");
    result.setRawHeader(QByteArray("Authorization"), "Bearer " + authToken.toUtf8());
    
    QSslConfiguration sslConfiguration = result.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    result.setSslConfiguration(sslConfiguration);
    
    return result;
}

bool BookStorageAccess::checkForErrors(int expectedStatusCode)
{
    if(m_reply->error() != QNetworkReply::NoError)
        qDebug() << "there was an error! " << m_reply->errorString();
    
    int statusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statusCode != expectedStatusCode)
    {
        qDebug() << "there was an error! " << m_reply->readAll();
        return true;
    }
    
    return false;
}

} // namespace infrastructure::persistence