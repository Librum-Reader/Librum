#include "book_service.hpp"
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QPixmap>
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
            this, &BookService::processBookCover);

    // Fetch changes timer
    m_fetchChangesTimer.setInterval(m_fetchChangedInterval);
    connect(&m_fetchChangesTimer, &QTimer::timeout, m_bookStorageManager,
            &IBookStorageManager::loadRemoteBooks);

    // Getting books finished
    connect(m_bookStorageManager,
            &IBookStorageManager::loadingRemoteBooksFinished, this,
            &BookService::updateLibrary);

    // Downloading book finished
    connect(m_bookStorageManager, &IBookStorageManager::finishedDownloadingBook,
            this, &BookService::processDownloadedBook);

    // Downloading book cover finished
    connect(m_bookStorageManager,
            &IBookStorageManager::finishedDownloadingBookCover, this,
            &BookService::processDownloadedBookCover);
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
    m_bookStorageManager->deleteBookCoverLocally(uuid);

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

    // Manually handle changes to "current page" because we don't want
    // "lastModified" to be updated on a "current page" change, since this
    // would break the whole updating mechanism.
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

BookOperationStatus BookService::changeBookCover(const QUuid& uuid,
                                                 const QString& filePath)
{
    auto* book = getBook(uuid);
    if(book == nullptr)
    {
        qWarning() << QString("Failed changing cover for book with uuid: %1."
                              "No book with this uuid exists.")
                          .arg(uuid.toString());
        return BookOperationStatus::BookDoesNotExist;
    }

    int index = getBookIndex(uuid);

    if(filePath.isEmpty())
    {
        // Delete the book cover
        m_bookStorageManager->deleteBookCoverLocally(uuid);

        book->setCoverPath("");
        book->setHasCover(false);
        book->updateCoverLastModified();

        emit dataChanged(index);
    }
    else
    {
        // Set new book cover
        auto absoluteFilePath = QUrl(filePath).path();
        QPixmap newCover(absoluteFilePath);
        if(newCover.isNull())
        {
            qWarning() << QString(
                              "Failed changing cover for book with uuid: %1."
                              "Can't open new image at: %2.")
                              .arg(uuid.toString(), absoluteFilePath);
            return BookOperationStatus::OperationFailed;
        }

        newCover =
            newCover.scaled(Book::maxCoverWidth, Book::maxCoverHeight,
                            Qt::KeepAspectRatio, Qt::SmoothTransformation);

        auto path = m_bookStorageManager->saveBookCoverToFile(uuid, newCover);
        if(!path.has_value())
        {
            qWarning() << QString(
                              "Failed changing cover for book with uuid: %1."
                              "Saving new image failed.")
                              .arg(uuid.toString(), absoluteFilePath);
            return BookOperationStatus::OperationFailed;
        }

        book->setHasCover(true);
        book->updateCoverLastModified();

        updateUIWithNewCover(uuid, path.value());
    }

    m_bookStorageManager->updateBook(*book);
    m_bookStorageManager->changeBookCover(*book);
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::addTagToBook(const QUuid& uuid,
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

BookOperationStatus BookService::removeTagFromBook(const QUuid& bookUuid,
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

BookOperationStatus BookService::renameTagOfBook(const QUuid& bookUuid,
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

bool BookService::refreshLastOpenedDateOfBook(const QUuid& uuid)
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

void BookService::processDownloadedBook(const QUuid& uuid,
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

void BookService::processDownloadedBookCover(const QUuid& uuid,
                                             const QString& filePath)
{
    auto* book = getBook(uuid);

    book->setCoverPath(filePath);
    book->setHasCover(true);
    m_bookStorageManager->updateBookLocally(*book);

    updateUIWithNewCover(uuid, filePath);
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
        checkIfBookFileStillExists(book);

        emit bookInsertionStarted(m_books.size());
        m_books.emplace_back(book);
        emit bookInsertionEnded();
    }
}

void BookService::checkIfBookFileStillExists(Book& book)
{
    // The file might have been moved or deleted from the user's filesystem
    QFile bookFile(QUrl(book.getFilePath()).path());
    if(!bookFile.exists())
    {
        // Delete the local book so the user can re-download it from the server
        book.setFilePath("");
        book.setDownloaded(false);
        m_bookStorageManager->deleteBookLocally(book.getUuid());
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

void BookService::processBookCover(const QPixmap* pixmap)
{
    // The book to assing the last cover to is always the last added book
    int index = m_books.size() - 1;
    auto& book = m_books.at(index);

    auto result =
        m_bookStorageManager->saveBookCoverToFile(book.getUuid(), *pixmap);
    if(!result.has_value())
    {
        qWarning() << QString("Failed creating cover for book with uuid: %1.")
                          .arg(book.getUuid().toString(QUuid::WithoutBraces));
        return;
    }

    book.updateCoverLastModified();
    book.setHasCover(true);
    book.setCoverPath(result.value());

    emit bookCoverGenerated(index);
}

void BookService::updateLibrary(std::vector<Book>& books)
{
    // The remote library is the library fetched from the server and the
    // local library is the library on the client's PC. On startup we need
    // to make sure that both libraries are synchronized.
    mergeRemoteLibraryIntoLocalLibrary(books);
    mergeLocalLibraryIntoRemoteLibrary(books);
}

void BookService::mergeRemoteLibraryIntoLocalLibrary(
    std::vector<Book>& remoteBooks)
{
    for(auto& remoteBook : remoteBooks)
    {
        auto* localBook = getBook(remoteBook.getUuid());
        if(localBook != nullptr)
        {
            mergeBooks(*localBook, remoteBook);
            continue;
        }


        // Unset cover path, since it is set by receiving slot
        if(remoteBook.hasCover())
            remoteBook.setCoverPath("");

        // Add the remote book to the local library if it does not exist
        emit bookInsertionStarted(m_books.size());
        m_books.emplace_back(remoteBook);
        emit bookInsertionEnded();

        // Get the cover for remote books which don't yet exist locally
        if(remoteBook.hasCover())
            m_bookStorageManager->getCoverForBook(remoteBook.getUuid());
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

void BookService::mergeBooks(Book& localBook, const Book& remoteBook)
{
    auto lastOpenedStatus = mergeCurrentPage(localBook, remoteBook);
    auto lastModifiedStatus = mergeBookData(localBook, remoteBook);
    auto coverLastModifiedStatus = mergeBookCover(localBook, remoteBook);


    if(lastOpenedStatus.localLibraryOutdated ||
       lastModifiedStatus.localLibraryOutdated ||
       coverLastModifiedStatus.localLibraryOutdated)
    {
        m_bookStorageManager->updateBookLocally(localBook);

        // Update UI
        auto index = getBookIndex(localBook.getUuid());
        emit dataChanged(index);
    }

    if(lastOpenedStatus.remoteLibraryOutdated ||
       lastModifiedStatus.remoteLibraryOutdated ||
       coverLastModifiedStatus.remoteLibraryOutdated)
    {
        m_bookStorageManager->updateBookRemotely(localBook);
    }
}

MergeStatus BookService::mergeCurrentPage(Book& localBook,
                                          const Book& remoteBook)
{
    // Take the current time in seconds, so that there are no ms mismatches
    auto localLastOpened = localBook.getLastOpened().toSecsSinceEpoch();
    auto remoteLastOpened = remoteBook.getLastOpened().toSecsSinceEpoch();

    // There are no "current page" differences between the local and remote
    // book
    if(remoteLastOpened == localLastOpened)
        return {};

    if(remoteLastOpened > localLastOpened)
    {
        localBook.setCurrentPage(remoteBook.getCurrentPage());
        localBook.setLastOpened(remoteBook.getLastOpened());

        return MergeStatus { .localLibraryOutdated = true };
    }

    return MergeStatus { .remoteLibraryOutdated = true };
}

MergeStatus BookService::mergeBookData(Book& localBook, const Book& remoteBook)
{
    // Take the current time in seconds, so that there are no ms mismatches
    auto localLastModified = localBook.getLastModified().toSecsSinceEpoch();
    auto remoteLastModified = remoteBook.getLastModified().toSecsSinceEpoch();

    // There are no data differences between the local and remote book
    if(remoteLastModified == localLastModified)
        return {};

    if(remoteLastModified > localLastModified)
    {
        // Save the file path since its overwritten during the update
        // operation
        auto localBookFilePath = localBook.getFilePath();
        localBook.update(remoteBook);
        localBook.setFilePath(localBookFilePath);

        return MergeStatus { .localLibraryOutdated = true };
    }

    return MergeStatus { .remoteLibraryOutdated = true };
}

MergeStatus BookService::mergeBookCover(Book& localBook, const Book& remoteBook)
{
    auto localCoverLastModified =
        localBook.getCoverLastModified().toSecsSinceEpoch();
    auto remoteCoverLastModified =
        remoteBook.getCoverLastModified().toSecsSinceEpoch();


    // There are no differences between the local and remote cover
    if(remoteCoverLastModified == localCoverLastModified)
        return {};

    if(remoteCoverLastModified > localCoverLastModified)
    {
        if(remoteBook.hasCover())
        {
            m_bookStorageManager->getCoverForBook(remoteBook.getUuid());
        }
        else
        {
            m_bookStorageManager->deleteBookCoverLocally(remoteBook.getUuid());

            localBook.setHasCover(false);
            localBook.updateCoverLastModified();
            updateUIWithNewCover(localBook.getUuid(), "");
        }

        return MergeStatus { .localLibraryOutdated = true };
    }

    m_bookStorageManager->changeBookCover(localBook);
    return MergeStatus { .remoteLibraryOutdated = true };
}

void BookService::updateUIWithNewCover(const QUuid& uuid, const QString& path)
{
    auto book = getBook(uuid);
    auto index = getBookIndex(uuid);

    // To properly update the UI, we need to invalidate the current image
    // and then set the new one.
    // We do this by setting the path to an empty string first and then to
    // the actual path. We emit 'dataChanged' in between to update the UI.
    book->setCoverPath("");
    emit dataChanged(index);

    book->setCoverPath(path);
    emit dataChanged(index);
}

}  // namespace application::services