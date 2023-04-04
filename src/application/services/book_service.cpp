#include "book_service.hpp"
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QTime>
#include <ranges>
#include "book_for_deletion.hpp"
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
    m_fetchChangesTimer.setInterval(m_fetchChangedInterval);
    connect(&m_fetchChangesTimer, &QTimer::timeout, m_bookStorageManager,
            &IBookStorageManager::loadRemoteBooks);

    // Getting books finished
    connect(m_bookStorageManager,
            &IBookStorageManager::loadingRemoteBooksFinished, this,
            &BookService::mergeLibraries);

    // Downloading book finished
    connect(m_bookStorageManager, &IBookStorageManager::finishedDownloadingBook,
            this, &BookService::updateDownloadedBook);
}

BookOperationStatus BookService::addBook(const QString& filePath)
{
    auto bookMetaData = m_bookMetadataHelper->getBookMetaData(filePath);
    if(!bookMetaData)
    {
        qWarning() << QString("Could not open book at path: %1 ").arg(filePath);
        return BookOperationStatus::OpeningBookFailed;
    }

    emit bookInsertionStarted(m_books.size());
    m_books.emplace_back(filePath, bookMetaData.value());
    emit bookInsertionEnded();

    // Generate cover after creating book to avoid adding cover to non-existent
    // book
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
    if(getBook(uuid) == nullptr)
    {
        qWarning() << QString("Could not delete book with uuid: %1. "
                              "No book with this uuid exists.")
                          .arg(uuid.toString());
        return BookOperationStatus::BookDoesNotExist;
    }

    const auto* book = getBook(uuid);
    utility::BookForDeletion bookToDelete {
        .uuid = uuid,
        .downloaded = book->getDownloaded(),
    };

    auto bookPosition = getBookPosition(uuid);
    int index = getBookIndex(uuid);

    emit bookDeletionStarted(index);
    m_books.erase(bookPosition);
    emit bookDeletionEnded();

    m_bookStorageManager->deleteBook(std::move(bookToDelete));

    return BookOperationStatus::Success;
}

BookOperationStatus BookService::uninstallBook(const QUuid& uuid)
{
    auto* book = getBook(uuid);
    if(book == nullptr)
    {
        qWarning() << QString("Could not uninstall book with uuid: %1. "
                              "No book with this uuid exists.")
                          .arg(uuid.toString());
        return BookOperationStatus::BookDoesNotExist;
    }

    m_bookStorageManager->uninstallBook(uuid);
    book->setDownloaded(false);

    int index = getBookIndex(uuid);
    emit dataChanged(index);

    return BookOperationStatus::Success;
}

BookOperationStatus BookService::downloadBook(const QUuid& uuid)
{
    auto* book = getBook(uuid);
    if(book == nullptr)
    {
        qWarning() << QString("Could not download book with uuid: %1. "
                              "No book with this uuid exists.")
                          .arg(uuid.toString());
        return BookOperationStatus::BookDoesNotExist;
    }

    m_bookStorageManager->downloadBook(uuid);
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::updateBook(const Book& newBook)
{
    auto* book = getBook(newBook.getUuid());
    if(book == nullptr)
    {
        qWarning() << QString("Failed updating book with uuid: %1."
                              "No book with this uuid exists.")
                          .arg(newBook.getUuid().toString());
        return BookOperationStatus::BookDoesNotExist;
    }

    // Manually handle changes to current page to prevent "lastModified" being
    // updated on a simple page update
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
    auto* book = getBook(uuid);
    if(book == nullptr)
    {
        qWarning() << QString("Adding tag to book with uuid: %1 failed. No "
                              "book with this uuid exists.")
                          .arg(uuid.toString());
        return BookOperationStatus::BookDoesNotExist;
    }

    if(!book->addTag(tag))
    {
        qWarning() << QString("Adding tag called: %1 to book with uuid: %2 "
                              "failed. A tag with this name already exists.")
                          .arg(tag.getName(), uuid.toString());
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
    auto* book = getBook(bookUuid);
    if(book == nullptr)
    {
        qWarning() << QString("Removing tag from book with uuid: %1 failed. No "
                              "book with this uuid exists.")
                          .arg(bookUuid.toString());
        return BookOperationStatus::BookDoesNotExist;
    }

    if(!book->removeTag(tagUuid))
    {
        qWarning() << QString("Removing tag with uuid: %1 from book with "
                              "uuid: %2 failed. No tag with this uuid exists.")
                          .arg(tagUuid.toString(), bookUuid.toString());
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
    auto* book = getBook(bookUuid);
    if(book == nullptr)
    {
        qWarning() << QString("Renaming tag from book with uuid: %1 failed."
                              "No book with this uuid exists.")
                          .arg(bookUuid.toString());
        return BookOperationStatus::BookDoesNotExist;
    }

    if(!book->renameTag(tagUuid, newName))
    {
        qWarning() << QString("Renaming tag with uuid: %1 from book with "
                              "uuid: %2 failed. No tag with this uuid exists "
                              "or a tag with this name already exists.")
                          .arg(tagUuid.toString(), bookUuid.toString());
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
    if(book == nullptr)
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
    auto* book = getBook(uuid);
    if(book == nullptr)
    {
        qWarning() << QString("Saving book with uuid: %1 to folder %2 failed."
                              " No book with this uuid exists.")
                          .arg(uuid.toString(), pathToFolder.path());
        return BookOperationStatus::BookDoesNotExist;
    }

    QUrl currentBookPath = book->getFilePath();
    QUrl destinaton = pathToFolder.path() + "/" + currentBookPath.fileName();

    auto result = QFile::copy(currentBookPath.path(), destinaton.path());
    if(!result)
    {
        qWarning() << QString("Saving book with uuid: %1 to folder: %2 failed. "
                              "No book with this uuid exists.")
                          .arg(uuid.toString(), pathToFolder.path());

        return BookOperationStatus::OperationFailed;
    }


    return BookOperationStatus::Success;
}

bool BookService::refreshLastOpened(const QUuid& uuid)
{
    auto* book = getBook(uuid);
    if(book == nullptr)
        return false;

    book->setLastOpened(QDateTime::currentDateTimeUtc());
    m_bookStorageManager->updateBook(*book);

    auto index = getBookIndex(uuid);
    emit dataChanged(index);

    return true;
}

void BookService::updateDownloadedBook(const QUuid& uuid,
                                       const QString& filePath)
{
    auto* book = getBook(uuid);

    book->setFilePath("file://" + filePath);
    book->setDownloaded(true);

    // The book meta-data file does not exist locally, so create it
    m_bookStorageManager->addBookLocally(*book);

    int index = getBookIndex(uuid);
    emit dataChanged(index);
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
        auto* localBook = getBook(remoteBook.getUuid());
        if(localBook != nullptr)
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
        bool localBookExistsOnServer = std::ranges::any_of(
            remoteBooks,
            [&localBook](const Book& remoteBook)
            {
                return remoteBook.getUuid() == localBook.getUuid();
            });

        if(!localBookExistsOnServer)
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
        auto index = getBookIndex(original.getUuid());
        emit dataChanged(index);
    }

    if(lastOpenedStatus.updateDatabase || lastModifiedStatus.updateDatabase)
    {
        m_bookStorageManager->updateBookRemotely(original);
    }
}

MergeStatus BookService::mergeCurrentPage(domain::entities::Book& original,
                                          const domain::entities::Book& mergee)
{
    // Take the current time in seconds, so there are no ms mismatches
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
    // Take the current time in seconds, so there are no ms mismatches
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