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

    connect(m_freeBooksStorageAccess,
            &IFreeBooksStorageAccess::gettingBookMediaChunkReady, this,
            &FreeBooksStorageGateway::gettingBookMediaChunkReady);
}

void FreeBooksStorageGateway::getBooksMetadata(const QString& author,
                                               const QString& title)
{
    m_freeBooksStorageAccess->getBooksMetadata(author, title);
}

void FreeBooksStorageGateway::getBookMedia(const QString& url,
                                           const QUuid& uuid)
{
    m_freeBooksStorageAccess->getBookMedia(url, uuid);
}

void FreeBooksStorageGateway::proccessBooksMetadata(const QByteArray& data)
{
    auto metadataObject = QJsonDocument::fromJson(data).object();
    auto results = metadataObject["results"].toArray();

    std::vector<FreeBook> books;
    for(const auto& result : results)
    {
        auto resultObject = result.toObject();
        auto isCopyrighted = resultObject["copyright"].toBool();
        if(isCopyrighted)
            continue;

        auto book = getBookFromJson(resultObject);

        books.emplace_back(std::move(book));
        getCoverForBook(resultObject);
    }

    emit gettingBooksMetaDataFinished(books);
}

FreeBook FreeBooksStorageGateway::getBookFromJson(const QJsonObject& values)
{
    auto authors = formatAuthors(values["authors"].toArray());
    auto languages = formatLanguages(values["languages"].toArray());

    FreeBook result {
        .id = values["id"].toInt(),
        .title = values["title"].toString(),
        .authors = authors,
        .languages = languages,
        .formats = "",
        .cover = QImage(),
        .downloadCount = values["download_count"].toInt(),
        .downloadLink = values["formats"]["application/epub+zip"].toString(),
    };
    return result;
}

QString FreeBooksStorageGateway::formatAuthors(QJsonArray authors)
{
    QString result;

    for(int i = 0; i < authors.size(); i++)
    {
        auto authorName = authors.at(i)["name"].toString();
        authorName.remove(',');
        result.append(authorName);

        if(i != authors.size() - 1)
            result.append(", ");
    }

    return result;
}

QString FreeBooksStorageGateway::formatLanguages(QJsonArray languages)
{
    QString result;
    for(int i = 0; i < languages.size(); i++)
    {
        auto language = languages[i].toString();
        result.append(language);

        if(i != languages.size() - 1)
            result.append(", ");
    }

    return result;
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
    emit gettingBookCoverFinished(bookId, QImage::fromData(data, "jpeg"));
}

}  // namespace adapters::gateways
