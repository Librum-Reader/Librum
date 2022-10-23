#include "book_storage_access.hpp"


using namespace adapters::dtos;


namespace infrastructure::persistence
{

BookStorageAccess::BookStorageAccess()
{
    
}


void BookStorageAccess::createBook(const QString& authToken,
                                   const BookDto& bookDto)
{
    Q_UNUSED(authToken);
    Q_UNUSED(bookDto);
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

} // namespace infrastructure::persistence