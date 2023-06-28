#include "free_books_storage_gateway.hpp"
#include <QJsonDocument>

using namespace domain::value_objects;

namespace adapters::gateways
{

FreeBooksStorageGateway::FreeBooksStorageGateway(
    IFreeBooksStorageAccess* freeBooksStorageAccess) :
    m_freeBooksStorageAccess(freeBooksStorageAccess)
{
    connect(m_freeBooksStorageAccess,
            &IFreeBooksStorageAccess::gettingBooksMetadataFinished, this,
            &FreeBooksStorageGateway::proccessBooksMetadata);

    connect(m_freeBooksStorageAccess,
            &IFreeBooksStorageAccess::gettingBookCoverFinished, this,
            &FreeBooksStorageGateway::proccessBookCover);
}

void FreeBooksStorageGateway::getBooksMetadata()
{
    m_freeBooksStorageAccess->getBooksMetadata();
}

void FreeBooksStorageGateway::proccessBooksMetadata(const QByteArray& data)
{
    auto metadataObject = QJsonDocument::fromJson(data).object();
    auto results = metadataObject["results"].toArray();

    std::vector<FreeBook> books;
    for(const auto& result : results)
    {
        auto resultObject = result.toObject();

        if(resultObject["copyright"].toBool())
            continue;

        FreeBook book;
        assignValuesToBook(book, resultObject);

        books.emplace_back(std::move(book));

        getCoverForBook(resultObject);
    }

    emit gettingBooksMetaDataFinished(books);
}

void FreeBooksStorageGateway::assignValuesToBook(FreeBook& book,
                                                 const QJsonObject& values)
{
    book.id = values["id"].toInt();
    book.title = values["title"].toString();
    book.mediaType = values["media_type"].toString();
    book.downloadCount = values["download_count"].toInt();

    addAuthorsToBook(book, values["authors"].toArray());
    addLanguagesToBook(book, values["languages"].toArray());
}

void FreeBooksStorageGateway::addAuthorsToBook(FreeBook& book,
                                               const QJsonArray& authors)
{
    QString authorNames;

    for(int i = 0; i < authors.size(); i++)
    {
        auto authorName = authors[i]["name"].toString();
        authorName.remove(',');

        authorNames.append(authorName);

        if(i != authors.size() - 1)
            authorNames.append(", ");
    }

    book.authors = authorNames;
}

void FreeBooksStorageGateway::addLanguagesToBook(FreeBook& book,
                                                 const QJsonArray& languages)
{
    QString languagesStr;

    for(int i = 0; i < languages.size(); i++)
    {
        auto language = languages[i].toString();

        languagesStr.append(language);

        if(i != languages.size() - 1)
            languagesStr.append(", ");
    }

    book.languages = languagesStr;
}

void FreeBooksStorageGateway::addFormatsToBook(FreeBook& book,
                                               const QJsonArray& formats)
{
}

void FreeBooksStorageGateway::getCoverForBook(const QJsonObject& book)
{
    auto bookId = book["id"].toInt();

    auto bookFormats = book["formats"].toObject();
    auto bookCoverUrl = bookFormats["image/jpeg"].toString();

    m_freeBooksStorageAccess->getCoverForBook(bookId, bookCoverUrl);
}

void FreeBooksStorageGateway::proccessBookCover(int bookId,
                                                const QByteArray& data)
{
    emit gettingBookCoverFinished(bookId, QImage::fromData(data));
}

}  // namespace adapters::gateways
