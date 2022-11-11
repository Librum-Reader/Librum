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

BookService::BookService(IBookStorageGateway* bookStorageGateway,
                         IBookMetadataHelper* bookMetadataHelper,
                         IDownloadedBooksTracker* downloadedBooksTracker,
                         IInternetConnectionInfo* internetConnectionInfo) :
    m_bookStorageGateway(bookStorageGateway),
    m_bookMetadataHelper(bookMetadataHelper),
    m_downloadedBooksTracker(downloadedBooksTracker),
    m_internetConnectionInfo(internetConnectionInfo)
{
    // Book cover generated
    connect(m_bookMetadataHelper, &IBookMetadataHelper::bookCoverGenerated,
            this, &BookService::storeBookCover);

    // Getting books finished
    connect(m_bookStorageGateway,
            &IBookStorageGateway::gettingBooksMetaDataFinished, this,
            &BookService::addRemoteBooks);
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

void BookService::addRemoteBooks(const std::vector<domain::models::Book>& books)
{
    for(const auto& remoteBook : books)
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

void BookService::mergeBooks(Book& original, const Book& toMerge)
{
    bool bookChanged = false;
    if(toMerge.getLastModified() > original.getLastModified())
    {
        auto localBookFilePath = original.getFilePath();
        original.update(toMerge);
        original.setFilePath(localBookFilePath);
        bookChanged = true;
    }

    if(toMerge.getLastOpened() > original.getLastOpened())
    {
        original.setCurrentPage(toMerge.getCurrentPage());
        bookChanged = true;
    }

    if(bookChanged)
    {
        // Update UI
        auto localBookIndex = getBookIndex(original.getUuid());
        emit dataChanged(localBookIndex);

        // Update local book
        m_downloadedBooksTracker->updateTrackedBook(original);
    }
}

}  // namespace application::services