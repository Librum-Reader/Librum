#include "book_storage_manager.hpp"

namespace application::utility
{

using namespace domain::models;

BookStorageManager::BookStorageManager(
    IBookStorageGateway* bookStorageGateway,
    IDownloadedBooksTracker* downloadedBooksTracker) :
    m_bookStorageGateway(bookStorageGateway),
    m_downloadedBooksTracker(downloadedBooksTracker)
{
}

void BookStorageManager::addBook(const Book& bookToAdd)
{
}

void BookStorageManager::deleteBook(const QUuid& uuid)
{
}

void BookStorageManager::uninstallBook(const QUuid& uuid)
{
}

void BookStorageManager::updateBook(const Book& newBook)
{
}

std::vector<Book> BookStorageManager::loadBooks()
{
}

}  // namespace application::utility