#include "book_service.hpp"
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QTime>
#include <ranges>
#include "book_operation_status.hpp"
#include "i_book_metadata_helper.hpp"

using namespace domain::entities;
using application::utility::MergeStatus;
using std::size_t;

namespace application::services
{

BookService::BookService(IBookMetadataHelper* bookMetadataHelper,
                         IBookStorageManager* bookStorageManager) :
    m_bookMetadataHelper(bookMetadataHelper),
    m_bookStorageManager(bookStorageManager)
{
    // Book cover generated
    connect(m_bookMetadataHelper, &IBookMetadataHelper::bookCoverGenerated,
            this, &BookService::storeBookCover);

    // Fetch changes timer
    m_fetchChangesTimer.setInterval(10'000);
    connect(&m_fetchChangesTimer, &QTimer::timeout, m_bookStorageManager,
            &IBookStorageManager::loadRemoteBooks);

    // Getting books finished
    connect(m_bookStorageManager,
            &IBookStorageManager::loadingRemoteBooksFinished, this,
            &BookService::mergeLibraries);
}

BookOperationStatus BookService::addBook(const QString& filePath)
{
    auto bookMetaData = m_bookMetadataHelper->getBookMetaData(filePath);
    if(!bookMetaData)
    {
        qWarning() << "Could not open book at path: " << filePath;
        return BookOperationStatus::OpeningBookFailed;
    }

    emit bookInsertionStarted(m_books.size());
    m_books.emplace_back(filePath, bookMetaData.value());
    emit bookInsertionEnded();

    // The cover needs to be generated after the book has been created,
    // else the cover is being added to a non existent book
    m_bookMetadataHelper->loadCover();

    const Book& bookToStore = m_books.at(m_books.size() - 1);
    m_bookStorageManager->addBook(bookToStore);

    return BookOperationStatus::Success;
}

auto BookService::getBookPosition(const QUuid& uuid)
{
    auto bookPosition = std::ranges::find_if(m_books,
                                             [&uuid](const Book& book)
                                             {
                                                 return book.getUuid() == uuid;
                                             });
    return bookPosition;
}

BookOperationStatus BookService::deleteBook(const QUuid& uuid)
{
    if(!getBook(uuid))
    {
        qWarning() << "Could not delete book with uuid: " << uuid
                   << ". The book was not found.";
        return BookOperationStatus::BookDoesNotExist;
    }

    auto bookPosition = getBookPosition(uuid);
    int index = getBookIndex(uuid);

    emit bookDeletionStarted(index);
    m_books.erase(bookPosition);
    emit bookDeletionEnded();

    m_bookStorageManager->deleteBook(uuid);

    return BookOperationStatus::Success;
}

BookOperationStatus BookService::uninstallBook(const QUuid& uuid)
{
    auto book = getBook(uuid);
    if(!book)
    {
        qWarning() << "Could not uninstall book with uuid: " << uuid
                   << ". No book with this uuid exists.";
        return BookOperationStatus::BookDoesNotExist;
    }

    int index = getBookIndex(uuid);

    m_bookStorageManager->uninstallBook(uuid);
    book->setDownloaded(false);
    emit dataChanged(index);

    return BookOperationStatus::Success;
}

BookOperationStatus BookService::updateBook(const Book& newBook)
{
    auto book = getBook(newBook.getUuid());
    if(!book)
    {
        qWarning() << "Could not update book with uuid: " << newBook.getUuid()
                   << ". No book with this uuid exists.";
        return BookOperationStatus::BookDoesNotExist;
    }

    // handle current page manually, so that "lastModified" doesnt get updated
    // when only the current page changes
    if(book->getCurrentPage() != newBook.getCurrentPage())
        book->setCurrentPage(newBook.getCurrentPage());

    if(*book != newBook)
    {
        book->update(newBook);
        book->updateLastModified();
    }

    int index = getBookIndex(newBook.getUuid());
    emit dataChanged(index);

    m_bookStorageManager->updateBook(*book);

    return BookOperationStatus::Success;
}

BookOperationStatus BookService::addTag(const QUuid& uuid,
                                        const domain::entities::Tag& tag)
{
    auto book = getBook(uuid);
    if(!book)
    {
        qWarning() << "Adding tag to book with uuid: " << uuid << " failed."
                   << "No book with this uuid exists.";
        return BookOperationStatus::BookDoesNotExist;
    }

    if(!book->addTag(tag))
    {
        qWarning() << "Adding tag called: " << tag.getName()
                   << " to book with uuid: " << uuid << " failed."
                   << "A tag with this name already exists.";
        return BookOperationStatus::TagAlreadyExists;
    }

    m_bookStorageManager->updateBook(*book);
    book->updateLastModified();

    int index = getBookIndex(uuid);
    emit tagsChanged(index);

    return BookOperationStatus::Success;
}

BookOperationStatus BookService::removeTag(const QUuid& bookUuid,
                                           const QUuid& tagUuid)
{
    auto book = getBook(bookUuid);
    if(!book)
    {
        qWarning() << "Removing tag from book with uuid: " << bookUuid
                   << " failed. No book with this uuid exists.";
        return BookOperationStatus::BookDoesNotExist;
    }

    if(!book->removeTag(tagUuid))
    {
        qWarning() << "Removing tag with uuid: " << tagUuid
                   << " from book with uuid: " << bookUuid << " failed."
                   << "No tag with this uuid exists.";
        return BookOperationStatus::TagDoesNotExist;
    }

    m_bookStorageManager->updateBook(*book);
    book->updateLastModified();

    int index = getBookIndex(bookUuid);
    emit tagsChanged(index);

    return BookOperationStatus::Success;
}

BookOperationStatus BookService::renameTag(const QUuid& bookUuid,
                                           const QUuid& tagUuid,
                                           const QString& newName)
{
    auto book = getBook(bookUuid);
    if(!book)
    {
        qWarning() << "Renaming tag from book with uuid: " << bookUuid
                   << " failed. No book with this uuid exists.";
        return BookOperationStatus::BookDoesNotExist;
    }

    if(!book->renameTag(tagUuid, newName))
    {
        qWarning() << "Renaming tag with uuid: " << tagUuid
                   << " from book with uuid: " << bookUuid << " failed."
                   << "No tag with this uuid exists or a tag with this name "
                      "already exists.";
        return BookOperationStatus::TagDoesNotExist;
    }

    // User service renames the tag remotely, just apply it locally
    m_bookStorageManager->updateBookLocally(*book);
    book->updateLastModified();

    int index = getBookIndex(bookUuid);
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

BookOperationStatus BookService::saveBookToFile(const QUuid& uuid,
                                                const QUrl& pathToFolder)
{
    auto book = getBook(uuid);
    if(!book)
    {
        qWarning() << "Saving book with uuid: " << uuid << " to file failed."
                   << " No book with this uuid exists.";
        return BookOperationStatus::BookDoesNotExist;
    }

    QUrl existingBook = book->getFilePath();
    QUrl newBook = pathToFolder.path() + "/" + existingBook.fileName();

    auto result = QFile::copy(existingBook.path(), newBook.path());
    if(!result)
    {
        qWarning() << "Saving book with uuid: " << uuid
                   << " to folder: " << pathToFolder.toLocalFile() << " failed."
                   << " No book with this uuid exists.";
        return BookOperationStatus::OperationFailed;
    }


    return BookOperationStatus::Success;
}

bool BookService::refreshLastOpened(const QUuid& uuid)
{
    auto book = getBook(uuid);
    if(!book)
        return false;

    book->setLastOpened(QDateTime::currentDateTimeUtc());
    m_bookStorageManager->updateBook(*book);

    auto index = getBookIndex(uuid);
    emit dataChanged(index);

    return true;
}

void BookService::setupUserData(const QString& token, const QString& email)
{
    m_bookStorageManager->setUserData(email, token);

    loadLocalBooks();
    m_bookStorageManager->loadRemoteBooks();

    m_fetchChangesTimer.start();
}

void BookService::loadLocalBooks()
{
    auto books = m_bookStorageManager->loadLocalBooks();
    for(auto book : books)
    {
        emit bookInsertionStarted(m_books.size());
        m_books.emplace_back(book);
        emit bookInsertionEnded();
    }
}

void BookService::clearUserData()
{
    m_bookStorageManager->clearUserData();
    m_fetchChangesTimer.stop();

    emit bookClearingStarted();
    m_books.clear();
    emit bookClearingEnded();
}

void BookService::storeBookCover(const QPixmap* pixmap)
{
    int index = m_books.size() - 1;
    auto& book = m_books.at(index);

    book.setCover(pixmap->toImage());
    emit bookCoverGenerated(index);
}

void BookService::mergeLibraries(
    const std::vector<domain::entities::Book>& books)
{
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
        auto remoteBookPos = std::ranges::find_if(
            remoteBooks,
            [&localBook](const Book& remoteBook)
            {
                return remoteBook.getUuid() == localBook.getUuid();
            });

        // Create a new book on the server if no remote book exists
        if(remoteBookPos == remoteBooks.end())
            m_bookStorageManager->addBook(localBook);
    }
}

void BookService::mergeBooks(Book& original, const Book& mergee)
{
    auto lastOpenedStatus = mergeCurrentPage(original, mergee);
    auto lastModifiedStatus = mergeBookData(original, mergee);


    if(lastOpenedStatus.updateLocalLibrary ||
       lastModifiedStatus.updateLocalLibrary)
    {
        m_bookStorageManager->updateBookLocally(original);

        // Update UI
        auto localBookIndex = getBookIndex(original.getUuid());
        emit dataChanged(localBookIndex);
    }

    if(lastOpenedStatus.updateDatabase || lastModifiedStatus.updateDatabase)
    {
        m_bookStorageManager->updateBookRemotely(original);
    }
}

MergeStatus BookService::mergeCurrentPage(domain::entities::Book& original,
                                          const domain::entities::Book& mergee)
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

MergeStatus BookService::mergeBookData(domain::entities::Book& original,
                                       const domain::entities::Book& mergee)
{
    // Take the current time in seconds, so there are no ms dismatches
    auto mergeeLastModified = mergee.getLastModified().toSecsSinceEpoch();
    auto originalLastModified = original.getLastModified().toSecsSinceEpoch();

    if(mergeeLastModified == originalLastModified)
        return {};

    if(mergeeLastModified > originalLastModified)
    {
        // Save the file path since its overwritten during the update
        // operation
        auto localBookFilePath = original.getFilePath();
        original.update(mergee);
        original.setFilePath(localBookFilePath);

        return MergeStatus { .updateLocalLibrary = true };
    }

    return MergeStatus { .updateDatabase = true };
}

}  // namespace application::services