#include "book_service.hpp"
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QTime>
#include <ranges>
#include "book_operation_status.hpp"
#include "i_book_metadata_helper.hpp"

namespace application::services
{

using namespace domain::models;
using std::size_t;
using utility::MergeStatus;

BookService::BookService(IBookStorageGateway* bookStorageGateway,
                         IBookMetadataHelper* bookMetadataHelper,
                         IDownloadedBooksTracker* downloadedBooksTracker,
                         IInternetConnectionInfo* internetConnectionInfo) :
    m_bookStorageGateway(bookStorageGateway),
    m_bookMetadataHelper(bookMetadataHelper),
    m_downloadedBooksTracker(downloadedBooksTracker),
    m_internetConnectionInfo(internetConnectionInfo)
{
    // Fetch changes timer
    m_fetchChangesTimer.setInterval(10'000);
    connect(&m_fetchChangesTimer, &QTimer::timeout, this,
            &BookService::loadRemoteBooks);

    // Book cover generated
    connect(m_bookMetadataHelper, &IBookMetadataHelper::bookCoverGenerated,
            this, &BookService::storeBookCover);

    // Getting books finished
    connect(m_bookStorageGateway,
            &IBookStorageGateway::gettingBooksMetaDataFinished, this,
            &BookService::mergeLibraries);
}

BookOperationStatus BookService::addBook(const QString& filePath)
{
    auto bookMetaData = m_bookMetadataHelper->getBookMetaData(filePath);
    if(!bookMetaData)
        return BookOperationStatus::OpeningBookFailed;

    emit bookInsertionStarted(m_books.size());
    m_books.emplace_back(filePath, bookMetaData.value());
    emit bookInsertionEnded();

    // The cover needs to be generated after the book has been created,
    // else the cover is being added to a non existent book
    m_bookMetadataHelper->loadCover();

    const Book& bookToStore = m_books.at(m_books.size() - 1);
    auto success = m_downloadedBooksTracker->trackBook(bookToStore);
    if(!success)
        return BookOperationStatus::OpeningBookFailed;

    m_bookStorageGateway->createBook(m_authenticationToken, bookToStore);

    return BookOperationStatus::Success;
}

BookOperationStatus BookService::deleteBook(const QUuid& uuid)
{
    if(!getBook(uuid))
        return BookOperationStatus::BookDoesNotExist;

    auto bookPosition = std::ranges::find_if(m_books,
                                             [&uuid](const Book& book)
                                             {
                                                 return book.getUuid() == uuid;
                                             });

    size_t index = getBookIndex(uuid);

    emit bookDeletionStarted(index);
    m_books.erase(bookPosition);
    emit bookDeletionEnded();

    m_downloadedBooksTracker->untrackBook(uuid);
    m_bookStorageGateway->deleteBook(m_authenticationToken, uuid);

    return BookOperationStatus::Success;
}

BookOperationStatus BookService::uninstallBook(const QUuid& uuid)
{
    auto book = getBook(uuid);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;

    size_t index = getBookIndex(uuid);

    m_downloadedBooksTracker->untrackBook(uuid);
    book->setDownloaded(false);
    emit dataChanged(index);

    return BookOperationStatus::Success;
}

BookOperationStatus BookService::updateBook(const Book& newBook)
{
    auto book = getBook(newBook.getUuid());
    if(!book)
        return BookOperationStatus::BookDoesNotExist;

    book->update(newBook);
    book->updateLastModified();

    int index = getBookIndex(newBook.getUuid());
    emit dataChanged(index);

    // Only try updating book in local-library if the book is downloaded
    if(book->getDownloaded())
        m_downloadedBooksTracker->updateTrackedBook(*book);

    m_bookStorageGateway->updateBook(m_authenticationToken, *book);

    return BookOperationStatus::Success;
}

BookOperationStatus BookService::addTag(const QUuid& uuid,
                                        const domain::models::Tag& tag)
{
    auto book = getBook(uuid);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;

    if(!book->addTag(tag))
        return BookOperationStatus::TagAlreadyExists;

    int index = getBookIndex(uuid);
    emit tagsChanged(index);

    return BookOperationStatus::Success;
}

BookOperationStatus BookService::removeTag(const QUuid& uuid,
                                           const domain::models::Tag& tag)
{
    auto book = getBook(uuid);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;

    if(!book->removeTag(tag))
        return BookOperationStatus::TagDoesNotExist;

    int index = getBookIndex(uuid);
    emit tagsChanged(index);

    return BookOperationStatus::Success;
}

const std::vector<Book>& BookService::getBooks() const
{
    return m_books;
}

const Book* BookService::getBook(const QUuid& uuid) const
{
    for(size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).getUuid() == uuid)
            return &(*(m_books.cbegin() + i));
    }

    return nullptr;
}

Book* BookService::getBook(const QUuid& uuid)
{
    for(size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).getUuid() == uuid)
            return &(*(m_books.begin() + i));
    }

    return nullptr;
}

int BookService::getBookIndex(const QUuid& uuid) const
{
    auto* book = getBook(uuid);
    if(!book)
        return -1;

    std::vector<Book>::const_iterator bookPosition(book);
    size_t index = bookPosition - m_books.begin();

    return index;
}

int BookService::getBookCount() const
{
    return m_books.size();
}

BookOperationStatus BookService::saveBookToPath(const QUuid& uuid,
                                                const QUrl& pathToFolder)
{
    auto book = getBook(uuid);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;

    QUrl existingBook = book->getFilePath();
    QUrl newBook = pathToFolder.path() + "/" + existingBook.fileName();

    auto result = QFile::copy(existingBook.path(), newBook.path());
    if(!result)
        return BookOperationStatus::OperationFailed;


    return BookOperationStatus::Success;
}

bool BookService::refreshLastOpened(const QUuid& uuid)
{
    auto book = getBook(uuid);
    if(!book)
        return false;

    book->setLastOpened(QDateTime::currentDateTimeUtc());
    m_downloadedBooksTracker->updateTrackedBook(*book);
    m_bookStorageGateway->updateBook(m_authenticationToken, *book);

    auto index = getBookIndex(uuid);
    emit dataChanged(index);

    return true;
}

void BookService::setAuthenticationToken(const QString& token,
                                         const QString& email)
{
    m_currentUserEmail = email;
    m_authenticationToken = token;

    loadBooks();
    m_fetchChangesTimer.start();
}

void BookService::clearAuthenticationToken()
{
    m_authenticationToken.clear();
}

void BookService::storeBookCover(const QPixmap* pixmap)
{
    int index = m_books.size() - 1;
    auto& book = m_books.at(index);

    book.setCover(pixmap->toImage());
    emit bookCoverGenerated(index);
}

void BookService::loadBooks()
{
    loadLocalBooks();

    // Only try loading remote books, if an internet connection exists
    m_internetConnectionInfo->checkAvailability();
    connect(m_internetConnectionInfo, &IInternetConnectionInfo::available, this,
            &BookService::loadRemoteBooks);
}

void BookService::loadLocalBooks()
{
    m_downloadedBooksTracker->setLibraryOwner(m_currentUserEmail);
    m_books = m_downloadedBooksTracker->getTrackedBooks();
}

void BookService::loadRemoteBooks()
{
    m_bookStorageGateway->getBooksMetaData(m_authenticationToken);
}

void BookService::mergeLibraries(const std::vector<domain::models::Book>& books)
{
    if(books.empty())
        return;

    mergeRemoteLibraryIntoLocalLibrary(books);
    mergeLocalLibraryIntoRemoteLibrary(books);
}

void BookService::mergeRemoteLibraryIntoLocalLibrary(
    const std::vector<Book>& remoteBooks)
{
    for(const auto& remoteBook : remoteBooks)
    {
        auto localBook = getBook(remoteBook.getUuid());
        if(localBook)
        {
            mergeBooks(*localBook, remoteBook);
            continue;
        }

        emit bookInsertionStarted(m_books.size());
        m_books.emplace_back(remoteBook);
        emit bookInsertionEnded();
    }
}

void BookService::mergeLocalLibraryIntoRemoteLibrary(
    const std::vector<Book>& remoteBooks)
{
    for(const auto& localBook : m_books)
    {
        auto remoteBook = std::ranges::find_if(
            remoteBooks,
            [&localBook](const Book& remoteBook)
            {
                return remoteBook.getUuid() == localBook.getUuid();
            });

        // Create a new book on the server if no remote book exists
        if(remoteBook == remoteBooks.end())
            m_bookStorageGateway->createBook(m_authenticationToken, localBook);
    }
}

void BookService::mergeBooks(Book& original, const Book& mergee)
{
    auto lastOpenedStatus = mergeCurrentPage(original, mergee);
    auto lastModifiedStatus = mergeBookData(original, mergee);


    if(lastOpenedStatus.updateLocalLibrary ||
       lastModifiedStatus.updateLocalLibrary)
    {
        m_downloadedBooksTracker->updateTrackedBook(original);

        // Update UI
        auto localBookIndex = getBookIndex(original.getUuid());
        emit dataChanged(localBookIndex);
    }

    if(lastOpenedStatus.updateDatabase || lastModifiedStatus.updateDatabase)
    {
        m_bookStorageGateway->updateBook(m_authenticationToken, original);
    }
}

MergeStatus BookService::mergeCurrentPage(domain::models::Book& original,
                                          const domain::models::Book& mergee)
{
    // Take the current time in seconds, so there are no ms dismatches
    auto mergeeLastOpened = mergee.getLastOpened().toSecsSinceEpoch();
    auto originalLastOpened = original.getLastOpened().toSecsSinceEpoch();

    if(mergeeLastOpened == originalLastOpened)
        return {};

    if(mergeeLastOpened > originalLastOpened)
    {
        original.setCurrentPage(mergee.getCurrentPage());
        original.setLastOpened(mergee.getLastOpened());

        return MergeStatus { .updateLocalLibrary = true };
    }

    return MergeStatus { .updateDatabase = true };
}

MergeStatus BookService::mergeBookData(domain::models::Book& original,
                                       const domain::models::Book& mergee)
{
    // Take the current time in seconds, so there are no ms dismatches
    auto mergeeLastModified = mergee.getLastModified().toSecsSinceEpoch();
    auto originalLastModified = original.getLastModified().toSecsSinceEpoch();

    if(mergeeLastModified == originalLastModified)
        return {};

    if(mergeeLastModified > originalLastModified)
    {
        // Save the file path since its overwritten during the update operation
        auto localBookFilePath = original.getFilePath();
        original.update(mergee);
        original.setFilePath(localBookFilePath);

        return MergeStatus { .updateLocalLibrary = true };
    }

    return MergeStatus { .updateDatabase = true };
}

}  // namespace application::services