#include "book_storage_gateway.hpp"
#include "book_dto.hpp"

using namespace adapters::dtos;


namespace adapters::gateways
{

BookStorageGateway::BookStorageGateway(IBookStorageAccess* bookStorageAccess)
    : m_bookStorageAccess(bookStorageAccess)
{
}


void BookStorageGateway::createBook(const QString& authToken, const domain::models::Book& book)
{
    BookDto bookDto
    {
        .uuid = book.getUuid().toString(QUuid::WithoutBraces),
        .title = book.getTitle(),
        .author = book.getAuthor(),
        .filePath = book.getFilePath(),
        .creator = book.getCreator(),
        .creationDate = book.getReleaseDate(),
        .format = book.getFormat(),
        .language = book.getLanguage(),
        .documentSize = book.getDocumentSize(),
        .pagesSize = book.getPagesSize(),
        .pageCount = book.getPageCount(),
        .currentPage = book.getCurrentPage(),
        .addedToLibrary = book.getAddedToLibrary(),
        .lastOpened = book.getLastOpened(),
        .cover = book.getCoverAsString()
    };
    
    m_bookStorageAccess->createBook(authToken, bookDto);
}

void BookStorageGateway::deleteBook(const QString& authToken, const QUuid& uuid)
{
    Q_UNUSED(authToken);
    Q_UNUSED(uuid);
}

void BookStorageGateway::updateBook(const QString& authToken, const domain::models::Book& book)
{
    Q_UNUSED(authToken);
    Q_UNUSED(book);
}

std::vector<domain::models::BookMetaData> BookStorageGateway::getBooksMetaData(const QString& authToken)
{
    Q_UNUSED(authToken);
}

void BookStorageGateway::downloadBook(const QString& authToken, const QUuid& uuid)
{
    Q_UNUSED(authToken);
    Q_UNUSED(uuid);
}

} // namespace adapters::gateways