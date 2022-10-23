#include "book_storage_gateway.hpp"


namespace adapters::gateways
{

BookStorageGateway::BookStorageGateway(IBookStorageAccess* bookStorageAccess)
    : m_bookStorageAccess(bookStorageAccess)
{
}


void BookStorageGateway::createBook(const QString& authToken, const domain::models::Book& book)
{
    Q_UNUSED(authToken);
    Q_UNUSED(book);
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