#include "book_storage_gateway.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include "book.hpp"
#include "i_book_storage_access.hpp"


using namespace domain::models;

namespace adapters::gateways
{

BookStorageGateway::BookStorageGateway(IBookStorageAccess* bookStorageAccess) :
    m_bookStorageAccess(bookStorageAccess)
{
    connect(m_bookStorageAccess,
            &IBookStorageAccess::gettingBooksMetaDataFinished, this,
            &BookStorageGateway::proccessBooksMetadata);
}

void BookStorageGateway::createBook(const QString& authToken,
                                    const domain::models::Book& book)
{
    auto jsonDoc = QJsonDocument::fromJson(book.toJson());
    auto jsonBook = jsonDoc.object();

    // Change the key name "uuid" to "guid" since that's what the api wants
    renameJsonObjectKey(jsonBook, "uuid", "guid");

    m_bookStorageAccess->createBook(authToken, jsonBook);
}

void BookStorageGateway::deleteBook(const QString& authToken, const QUuid& uuid)
{
    m_bookStorageAccess->deleteBook(authToken, uuid);
}

void BookStorageGateway::updateBook(const QString& authToken,
                                    const domain::models::Book& book)
{
    auto jsonDoc = QJsonDocument::fromJson(book.toJson());
    auto jsonBook = jsonDoc.object();

    m_bookStorageAccess->updateBook(authToken, jsonBook);
}

void BookStorageGateway::getBooksMetaData(const QString& authToken)
{
    m_bookStorageAccess->getBooksMetaData(authToken);
}

void BookStorageGateway::downloadBook(const QString& authToken,
                                      const QUuid& uuid)
{
    Q_UNUSED(authToken);
    Q_UNUSED(uuid);
}

void BookStorageGateway::addTag(const QString& authToken,
                                const domain::models::Tag& book)
{
}

void BookStorageGateway::deleteTag(const QString& authToken, const QUuid& uuid)
{
}

void BookStorageGateway::proccessBooksMetadata(
    std::vector<QJsonObject>& jsonBooks)
{
    std::vector<Book> books;
    for(auto& jsonBook : jsonBooks)
    {
        // Rename the key name "guid" to "uuid" since that's what the core wants
        renameJsonObjectKey(jsonBook, "guid", "uuid");

        auto book = Book::fromJson(jsonBook);
        book.setDownloaded(false);

        books.emplace_back(std::move(book));
    }

    emit gettingBooksMetaDataFinished(books);
}

void BookStorageGateway::renameJsonObjectKey(QJsonObject& jsonObject,
                                             const QString& oldKeyName,
                                             const QString& newKeyName)
{
    auto temp = jsonObject[oldKeyName].toString();
    jsonObject.remove(oldKeyName);
    jsonObject.insert(newKeyName, temp);
}

}  // namespace adapters::gateways