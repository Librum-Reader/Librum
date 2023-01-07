#include "book_storage_gateway.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "book.hpp"
#include "i_book_storage_access.hpp"


using namespace domain::entities;

namespace adapters::gateways
{

BookStorageGateway::BookStorageGateway(IBookStorageAccess* bookStorageAccess) :
    m_bookStorageAccess(bookStorageAccess)
{
    connect(m_bookStorageAccess,
            &IBookStorageAccess::gettingBooksMetaDataFinished, this,
            &BookStorageGateway::proccessBooksMetadata);
}

void BookStorageGateway::createBook(const QString& authToken, const Book& book)
{
    auto jsonDoc = QJsonDocument::fromJson(book.toJson());
    auto jsonBook = jsonDoc.object();

    // Change the key name "uuid" to "guid" since that's what the api requests
    renameJsonObjectKey(jsonBook, "uuid", "guid");

    // Again, change "uuid" to "guid" for every tag for the api request
    auto tags = jsonBook["tags"].toArray();
    auto fixedTags = renameTagProperties(tags, TagNamingStyle::ApiStyle);
    jsonBook["tags"] = fixedTags;

    m_bookStorageAccess->createBook(authToken, jsonBook);
}

void BookStorageGateway::deleteBook(const QString& authToken, const QUuid& uuid)
{
    m_bookStorageAccess->deleteBook(authToken, uuid);
}

void BookStorageGateway::updateBook(const QString& authToken, const Book& book)
{
    auto jsonDoc = QJsonDocument::fromJson(book.toJson());
    auto jsonBook = jsonDoc.object();

    // Change the key name "uuid" to "guid" since that's what the api requests
    renameJsonObjectKey(jsonBook, "uuid", "guid");

    // Api wants the "uuid" to be called "guid", so rename it
    auto tags = jsonBook["tags"].toArray();
    auto fixedTags = renameTagProperties(tags, TagNamingStyle::ApiStyle);
    jsonBook["tags"] = fixedTags;

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

void BookStorageGateway::proccessBooksMetadata(
    std::vector<QJsonObject>& jsonBooks)
{
    std::vector<Book> books;
    for(auto& jsonBook : jsonBooks)
    {
        // Api sends "uuid" by the name of "guid", so rename it back to "uuid"
        renameJsonObjectKey(jsonBook, "guid", "uuid");

        // Rename "guid"s to "uuid"s for tags as well
        auto tags = jsonBook["tags"].toArray();
        auto fixedTags = renameTagProperties(tags, TagNamingStyle::ClientStyle);
        jsonBook["tags"] = fixedTags;


        auto book = Book::fromJson(jsonBook);
        book.setDownloaded(false);

        books.emplace_back(std::move(book));
    }

    emit gettingBooksMetaDataFinished(books);
}

QJsonArray BookStorageGateway::renameTagProperties(const QJsonArray& tags,
                                                   TagNamingStyle namingStyle)
{
    QJsonArray newTags;
    for(const QJsonValue& tag : tags)
    {
        auto tagObject = tag.toObject();

        if(namingStyle == TagNamingStyle::ApiStyle)
            renameJsonObjectKey(tagObject, "uuid", "guid");
        else
            renameJsonObjectKey(tagObject, "guid", "uuid");

        newTags.append(QJsonValue::fromVariant(tagObject));
    }

    return newTags;
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