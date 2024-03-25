#include "library_storage_gateway.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "book.hpp"
#include "i_library_storage_access.hpp"


using namespace domain::entities;

namespace adapters::gateways
{

LibraryStorageGateway::LibraryStorageGateway(
    ILibraryStorageAccess* bookStorageAccess) :
    m_bookStorageAccess(bookStorageAccess)
{
    // Loading books
    connect(m_bookStorageAccess,
            &ILibraryStorageAccess::gettingBooksMetaDataFinished, this,
            &LibraryStorageGateway::proccessBooksMetadata);

    // Save downloaded book
    connect(m_bookStorageAccess,
            &ILibraryStorageAccess::downloadingBookMediaChunkReady, this,
            &LibraryStorageGateway::downloadingBookMediaChunkReady);

    // Downloading book media progress
    connect(m_bookStorageAccess,
            &ILibraryStorageAccess::downloadingBookMediaProgressChanged, this,
            &LibraryStorageGateway::downloadingBookMediaProgressChanged);

    // Save book cover
    connect(m_bookStorageAccess,
            &ILibraryStorageAccess::downloadingBookCoverFinished, this,
            &LibraryStorageGateway::downloadingBookCoverFinished);

    // Storage limit exceeded
    connect(m_bookStorageAccess, &ILibraryStorageAccess::storageLimitExceeded,
            this, &LibraryStorageGateway::storageLimitExceeded);

    // Book upload succeeded
    connect(m_bookStorageAccess, &ILibraryStorageAccess::bookUploadSucceeded,
            this, &LibraryStorageGateway::bookUploadSucceeded);
}

void LibraryStorageGateway::createBook(const QString& authToken,
                                       const Book& book)
{
    auto jsonDoc = QJsonDocument::fromJson(book.toJson());
    auto jsonBook = jsonDoc.object();

    convertJsonBookToApiFormat(jsonBook);

    m_bookStorageAccess->createBook(authToken, jsonBook);
}

void LibraryStorageGateway::deleteBook(const QString& authToken,
                                       const QUuid& uuid)
{
    m_bookStorageAccess->deleteBook(authToken, uuid);
}

void LibraryStorageGateway::updateBook(const QString& authToken,
                                       const Book& book)
{
    auto jsonDoc = QJsonDocument::fromJson(book.toJson());
    auto jsonBook = jsonDoc.object();

    convertJsonBookToApiFormat(jsonBook);

    m_bookStorageAccess->updateBook(authToken, jsonBook);
}

void LibraryStorageGateway::changeBookCover(const QString& authToken,
                                            const QUuid& uuid,
                                            const QString& path)
{
    m_bookStorageAccess->uploadBookCover(authToken, uuid, path);
}

void LibraryStorageGateway::deleteBookCover(const QString& authToken,
                                            const QUuid& uuid)
{
    m_bookStorageAccess->deleteBookCover(authToken, uuid);
}

void LibraryStorageGateway::getBooksMetaData(const QString& authToken)
{
    m_bookStorageAccess->getBooksMetaData(authToken);
}

void LibraryStorageGateway::getCoverForBook(const QString& authToken,
                                            const QUuid& uuid)
{
    m_bookStorageAccess->downloadCoverForBook(authToken, uuid);
}

void LibraryStorageGateway::downloadBookMedia(const QString& authToken,
                                              const QUuid& uuid)
{
    m_bookStorageAccess->downloadBookMedia(authToken, uuid);
}

void LibraryStorageGateway::proccessBooksMetadata(
    std::vector<QJsonObject>& jsonBooks, bool success)
{
    std::vector<Book> books;
    if(!success)
    {
        emit gettingBooksMetaDataFinished(books, false);
        return;
    }

    for(auto& jsonBook : jsonBooks)
    {
        // Api sends "uuid" by the name of "guid", so rename it back to "uuid"
        renameJsonObjectKey(jsonBook, "guid", "uuid");

        // Rename "guid"s to "uuid"s for tags as well
        auto tags = jsonBook["tags"].toArray();
        auto fixedTags = renameProperties(tags, TagNamingStyle::ClientStyle);
        jsonBook["tags"] = fixedTags;

        // Highlights
        auto highlightsToFix = jsonBook["highlights"].toArray();
        auto fixedHighlights =
            renameProperties(highlightsToFix, TagNamingStyle::ClientStyle);

        // Highlights' rects
        for(auto highlight : fixedHighlights)
        {
            auto highlightObject = highlight.toObject();
            auto rectsToFix = highlightObject["rects"].toArray();
            auto fixedRects =
                renameProperties(rectsToFix, TagNamingStyle::ClientStyle);
            highlightObject["rects"] = fixedRects;

            highlight = highlightObject;
        }

        jsonBook["highlights"] = fixedHighlights;

        auto book = Book::fromJson(jsonBook);
        book.setDownloaded(false);

        books.emplace_back(std::move(book));
    }

    emit gettingBooksMetaDataFinished(books, true);
}

void LibraryStorageGateway::convertJsonBookToApiFormat(QJsonObject& jsonBook)
{
    // Change the json key names from "uuid" to "guid" since that's what the api
    // requests
    renameJsonObjectKey(jsonBook, "uuid", "guid");

    // Tags
    auto tagsToFix = jsonBook["tags"].toArray();
    auto fixedTags = renameProperties(tagsToFix, TagNamingStyle::ApiStyle);
    jsonBook["tags"] = fixedTags;

    // Bookmarks
    auto bookmarksToFix = jsonBook["bookmarks"].toArray();
    auto fixedBookmarks =
        renameProperties(bookmarksToFix, TagNamingStyle::ApiStyle);
    jsonBook["bookmarks"] = fixedBookmarks;

    // Highlights
    auto highlightsToFix = jsonBook["highlights"].toArray();
    auto fixedHighlights =
        renameProperties(highlightsToFix, TagNamingStyle::ApiStyle);

    // Highlights' rects
    for(auto highlight : fixedHighlights)
    {
        auto highlightObject = highlight.toObject();
        auto rectsToFix = highlightObject["rects"].toArray();
        auto fixedRects =
            renameProperties(rectsToFix, TagNamingStyle::ApiStyle);
        highlightObject["rects"] = fixedRects;

        highlight = highlightObject;
    }

    jsonBook["highlights"] = fixedHighlights;
}

QJsonArray LibraryStorageGateway::renameProperties(const QJsonArray& items,
                                                   TagNamingStyle namingStyle)
{
    QJsonArray newItems;
    for(const QJsonValue& item : items)
    {
        auto tagObject = item.toObject();

        if(namingStyle == TagNamingStyle::ApiStyle)
            renameJsonObjectKey(tagObject, "uuid", "guid");
        else
            renameJsonObjectKey(tagObject, "guid", "uuid");

        newItems.append(QJsonValue::fromVariant(tagObject));
    }

    return newItems;
}

void LibraryStorageGateway::renameJsonObjectKey(QJsonObject& jsonObject,
                                                const QString& oldKeyName,
                                                const QString& newKeyName)
{
    auto temp = jsonObject[oldKeyName].toString();
    jsonObject.remove(oldKeyName);
    jsonObject.insert(newKeyName, temp);
}

}  // namespace adapters::gateways
