#include "book_storage_manager.hpp"
#include <vector>

using namespace domain::entities;

namespace application::utility
{

BookStorageManager::BookStorageManager(
    IBookStorageGateway* bookStorageGateway,
    IDownloadedBooksTracker* downloadedBooksTracker) :
    m_bookStorageGateway(bookStorageGateway),
    m_downloadedBooksTracker(downloadedBooksTracker)
{
    connect(m_bookStorageGateway,
            &IBookStorageGateway::gettingBooksMetaDataFinished, this,
            [this](const std::vector<Book>& books)
            {
                // Avoid storing books for logged out users by verifying login
                // status before adding books, else books might get loaded into
                // memory, even though the user is logged out.
                if(!userLoggedIn())
                    return;

                emit loadingRemoteBooksFinished(books);
            });
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

bool BookStorageManager::userLoggedIn()
{
    return !m_authenticationToken.isEmpty();
}

void BookStorageManager::addBook(const Book& bookToAdd)
{
    // Prevent adding remote books to the local library unless "downloaded" is
    // set to true.
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

void BookStorageManager::updateBookLocally(const domain::entities::Book& book)
{
    // Prevent updating remote books in the local library unless the book is
    // downloaded. If its not downloaded, there is no local file to update.
    if(book.getDownloaded())
        m_downloadedBooksTracker->updateTrackedBook(book);
}

void BookStorageManager::updateBookRemotely(const domain::entities::Book& book)
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

}  // namespace application::utility