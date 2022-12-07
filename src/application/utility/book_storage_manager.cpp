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
    connect(m_bookStorageGateway,
            &IBookStorageGateway::gettingBooksMetaDataFinished, this,
            &IBookStorageManager::loadingRemoteBooksFinished);
}

void BookStorageManager::setUserData(const QString& email,
                                     const QString& authToken)
{
    m_authenticationToken = authToken;
    m_downloadedBooksTracker->setLibraryOwner(email);
}

void BookStorageManager::clearUserData()
{
    m_authenticationToken.clear();
    m_downloadedBooksTracker->clearLibraryOwner();
}

void BookStorageManager::addBook(const Book& bookToAdd)
{
    // Prevent adding remote books to the local library if not specified to
    // do so by settting "downloaded" to true.
    if(bookToAdd.getDownloaded())
        m_downloadedBooksTracker->trackBook(bookToAdd);

    m_bookStorageGateway->createBook(m_authenticationToken, bookToAdd);
}

void BookStorageManager::deleteBook(const QUuid& uuid)
{
    m_downloadedBooksTracker->untrackBook(uuid);
    m_bookStorageGateway->deleteBook(m_authenticationToken, uuid);
}

void BookStorageManager::uninstallBook(const QUuid& uuid)
{
    m_downloadedBooksTracker->untrackBook(uuid);
}

void BookStorageManager::updateBook(const Book& book)
{
    updateBookLocally(book);
    updateBookRemotely(book);
}

void BookStorageManager::updateBookLocally(const domain::models::Book& book)
{
    // Only try updating book in local-library if the book is downloaded
    if(book.getDownloaded())
        m_downloadedBooksTracker->updateTrackedBook(book);
}

void BookStorageManager::updateBookRemotely(const domain::models::Book& book)
{
    m_bookStorageGateway->updateBook(m_authenticationToken, book);
}

std::vector<Book> BookStorageManager::loadLocalBooks()
{
    auto m_localBooks = m_downloadedBooksTracker->getTrackedBooks();
    return m_localBooks;
}

void BookStorageManager::loadRemoteBooks()
{
    m_bookStorageGateway->getBooksMetaData(m_authenticationToken);
}

void BookStorageManager::addTag(const domain::models::Tag& tag)
{
    
}

void BookStorageManager::deleteTag(const QUuid& uuid)
{
    
}

}  // namespace application::utility