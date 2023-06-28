#include "free_books_storage_gateway.hpp"
#include <QJsonDocument>

using namespace domain::entities;

namespace adapters::gateways
{

FreeBooksStorageGateway::FreeBooksStorageGateway(
    IFreeBooksStorageAccess* freeBooksStorageAccess) :
    m_freeBooksStorageAccess(freeBooksStorageAccess)
{
    connect(m_freeBooksStorageAccess,
            &IFreeBooksStorageAccess::gettingBooksMetadataFinished, this,
            &FreeBooksStorageGateway::proccessBooksMetadata);
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
    }

    emit gettingBooksMetaDataFinished(books);
}

void FreeBooksStorageGateway::assignValuesToBook(FreeBook& book,
                                                 const QJsonObject& values)
{
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

}  // namespace adapters::gateways
