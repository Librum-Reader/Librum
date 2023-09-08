#include "library_service.hpp"
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QPixmap>
#include <QTime>
#include <ranges>
#include "book_for_deletion.hpp"
#include "book_merger.hpp"
#include "book_operation_status.hpp"
#include "i_metadata_extractor.hpp"

using namespace domain::entities;
using std::size_t;

namespace application::services
{

LibraryService::LibraryService(IMetadataExtractor* bookMetadataHelper,
                               ILibraryStorageManager* bookStorageManager) :
    m_bookMetadataHelper(bookMetadataHelper),
    m_bookStorageManager(bookStorageManager)
{
    // Fetch changes timer
    m_fetchChangesTimer.setInterval(m_fetchChangedInterval);
    connect(&m_fetchChangesTimer, &QTimer::timeout, m_bookStorageManager,
            &ILibraryStorageManager::downloadRemoteBooks);

    // Getting books finished
    connect(m_bookStorageManager,
            &ILibraryStorageManager::finishedDownloadingRemoteBooks, this,
            &LibraryService::updateLibrary);

    // Downloading book media progress
    connect(m_bookStorageManager,
            &ILibraryStorageManager::downloadingBookMediaProgressChanged, this,
            &LibraryService::setMediaDownloadProgressForBook);

    // Downloading book finished
    connect(m_bookStorageManager,
            &ILibraryStorageManager::finishedDownloadingBookMedia, this,
            &LibraryService::processDownloadedBook);

    // Downloading book cover finished
    connect(m_bookStorageManager,
            &ILibraryStorageManager::finishedDownloadingBookCover, this,
            &LibraryService::processDownloadedBookCover);

    // Storage limit exceeded
    connect(m_bookStorageManager, &ILibraryStorageManager::storageLimitExceeded,
            this, &LibraryService::storageLimitExceeded);

    // Book upload succeeded
    connect(m_bookStorageManager, &ILibraryStorageManager::bookUploadSucceeded,
            this,
            [this](const QUuid& uuid)
            {
                auto book = getBook(uuid);
                book->setExistsOnlyOnClient(false);
                m_bookStorageManager->updateBookLocally(*book);
                refreshUIForBook(uuid);
            });
}

void LibraryService::downloadBooks()
{
    m_bookStorageManager->downloadRemoteBooks();
}

BookOperationStatus LibraryService::addBook(const QString& filePath)
{
    auto success = m_bookMetadataHelper->setup(filePath);
    if(!success)
    {
        qWarning() << QString("Could not open book at path: %1 ").arg(filePath);
        return BookOperationStatus::OpeningBookFailed;
    }

    auto bookMetaData = m_bookMetadataHelper->getBookMetaData();
    Book book(filePath, bookMetaData);

    auto cover = m_bookMetadataHelper->getBookCover();
    cover = cover.scaled(Book::maxCoverWidth, Book::maxCoverHeight,
                         Qt::KeepAspectRatio, Qt::SmoothTransformation);
    auto coverPath =
        m_bookStorageManager->saveBookCoverToFile(book.getUuid(), cover);
    if(coverPath.isEmpty())
    {
        qWarning() << QString("Failed creating cover for book with uuid: %1.")
                          .arg(book.getUuid().toString(QUuid::WithoutBraces));
        return BookOperationStatus::OperationFailed;
    }

    book.updateCoverLastModified();
    book.setHasCover(true);
    book.setCoverPath(coverPath);

    addBookToLibrary(book);

    m_bookStorageManager->addBook(book);
    return BookOperationStatus::Success;
}

auto LibraryService::getBookPosition(const QUuid& uuid)
{
    auto bookPosition = std::ranges::find_if(m_books,
                                             [&uuid](const Book& book)
                                             {
                                                 return book.getUuid() == uuid;
                                             });
    return bookPosition;
}

BookOperationStatus LibraryService::deleteBook(const QUuid& uuid)
{
    const auto* book = getBook(uuid);
    if(!book)
    {
        qWarning() << QString("Could not delete book with uuid: %1. "
                              "No book with this uuid exists.")
                          .arg(uuid.toString());
        return BookOperationStatus::BookDoesNotExist;
    }


    utility::BookForDeletion bookToDelete {
        .uuid = book->getUuid(),
        .downloaded = book->isDownloaded(),
        .format = book->getFormat(),
    };

    auto bookPosition = getBookPosition(uuid);
    int index = getBookIndex(uuid);

    emit bookDeletionStarted(index);
    m_books.erase(bookPosition);
    emit bookDeletionEnded();

    m_bookStorageManager->deleteBook(std::move(bookToDelete));
    return BookOperationStatus::Success;
}

BookOperationStatus LibraryService::deleteAllBooks()
{
    m_bookStorageManager->deleteAllBooks();
    return BookOperationStatus::Success;
}

BookOperationStatus LibraryService::uninstallBook(const QUuid& uuid)
{
    auto* book = getBook(uuid);
    if(book == nullptr)
    {
        qWarning() << QString("Could not uninstall book with uuid: %1. "
                              "No book with this uuid exists.")
                          .arg(uuid.toString());
        return BookOperationStatus::BookDoesNotExist;
    }

    m_bookStorageManager->uninstallBook(*book);
    book->setDownloaded(false);

    refreshUIForBook(uuid);
    return BookOperationStatus::Success;
}

BookOperationStatus LibraryService::downloadBookMedia(const QUuid& uuid)
{
    auto* book = getBook(uuid);
    if(book == nullptr)
    {
        qWarning() << QString("Could not download book media with uuid: %1. "
                              "No book with this uuid exists.")
                          .arg(uuid.toString());
        return BookOperationStatus::BookDoesNotExist;
    }

    m_bookStorageManager->downloadBookMedia(uuid);
    return BookOperationStatus::Success;
}

BookOperationStatus LibraryService::updateBook(const Book& newBook)
{
    auto* book = getBook(newBook.getUuid());
    if(book == nullptr)
    {
        qWarning() << QString("Failed updating book with uuid: %1."
                              "No book with this uuid exists.")
                          .arg(newBook.getUuid().toString());
        return BookOperationStatus::BookDoesNotExist;
    }

    if(!newBook.isValid())
    {
        qWarning() << QString("Failed updating book with uuid: %1. "
                              "The new properties are invalid.")
                          .arg(newBook.getUuid().toString());
        return BookOperationStatus::OperationFailed;
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

    m_bookStorageManager->updateBook(*book);
    refreshUIForBook(newBook.getUuid());
    return BookOperationStatus::Success;
}

BookOperationStatus LibraryService::changeBookCover(const QUuid& uuid,
                                                    const QString& filePath)
{
    auto* book = getBook(uuid);
    if(book == nullptr)
    {
        qWarning() << QString("Failed changing cover for book with uuid: %1. "
                              "No book with this uuid exists.")
                          .arg(uuid.toString());
        return BookOperationStatus::BookDoesNotExist;
    }

    // An empty file path indicates that the book cover shall be deleted.
    if(filePath.isEmpty())
    {
        deleteBookCover(*book);
    }
    else
    {
        auto success = setNewBookCover(*book, filePath);
        if(!success)
            return BookOperationStatus::OperationFailed;
    }

    m_bookStorageManager->updateBook(*book);
    m_bookStorageManager->updateBookCoverRemotely(book->getUuid(),
                                                  book->hasCover());
    return BookOperationStatus::Success;
}

void LibraryService::deleteBookCover(Book& book)
{
    auto success = m_bookStorageManager->deleteBookCoverLocally(book.getUuid());
    if(!success)
    {
        qWarning() << QString("Failed deleting the local book cover for book "
                              "with uuid: %1. "
                              "Deleting the file failed.")
                          .arg(book.getUuid().toString(QUuid::WithoutBraces));
    }

    book.setCoverPath("");
    book.setHasCover(false);
    book.updateCoverLastModified();

    refreshUIForBook(book.getUuid());
}

bool LibraryService::setNewBookCover(Book& book, QString filePath)
{
    auto uuid = book.getUuid();
    QFileInfo newCoverFile(filePath);
    if(!newCoverFile.exists() || !newCoverFile.isFile())
    {
        qWarning() << QString(
                          "Failed setting new cover for book with uuid: %1. "
                          "The given file at: %2 is invalid.")
                          .arg(uuid.toString(), filePath);
        return false;
    }

    QImage newCover(filePath);
    if(newCover.isNull())
    {
        qWarning() << QString(
                          "Failed setting new cover for book with uuid: %1. "
                          "Can't open new cover image at: %2.")
                          .arg(uuid.toString(), filePath);
        return false;
    }

    // Scale the book cover to the correct size
    newCover = newCover.scaled(Book::maxCoverWidth, Book::maxCoverHeight,
                               Qt::KeepAspectRatio, Qt::SmoothTransformation);

    auto path = m_bookStorageManager->saveBookCoverToFile(uuid, newCover);
    if(path.isEmpty())
    {
        qWarning() << QString(
                          "Failed setting new cover for book with uuid: %1. "
                          "Saving new cover image failed.")
                          .arg(uuid.toString(), filePath);
        return false;
    }

    book.setHasCover(true);
    book.updateCoverLastModified();

    refreshUIWithNewCover(uuid, path);
    return true;
}

void LibraryService::addBookToLibrary(const Book& book)
{
    emit bookInsertionStarted(m_books.size());
    m_books.emplace_back(book);
    emit bookInsertionEnded();
}

void LibraryService::setMediaDownloadProgressForBook(const QUuid& uuid,
                                                     qint64 bytesReceived,
                                                     qint64 bytesTotal)
{
    auto* book = getBook(uuid);
    if(book == nullptr)
    {
        qWarning() << QString("Failed setting media download progress for book "
                              "with uuid: %1. No book with this uuid exists.")
                          .arg(uuid.toString());
        return;
    }

    auto progress = static_cast<double>(bytesReceived) / bytesTotal;
    book->setMediaDownloadProgress(progress);
    emit downloadingBookMediaProgressChanged(getBookIndex(uuid));
}

BookOperationStatus LibraryService::addTagToBook(
    const QUuid& uuid, const domain::entities::Tag& tag)
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

BookOperationStatus LibraryService::removeTagFromBook(const QUuid& bookUuid,
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

BookOperationStatus LibraryService::renameTagOfBook(const QUuid& bookUuid,
                                                    const QUuid& tagUuid,
                                                    const QString& newName)
{
    auto* book = getBook(bookUuid);
    if(book == nullptr)
    {
        qWarning() << QString("Renaming tag from book with uuid: %1 failed. "
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

const std::vector<Book>& LibraryService::getBooks() const
{
    return m_books;
}

const Book* LibraryService::getBook(const QUuid& uuid) const
{
    for(size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).getUuid() == uuid)
            return &(*(m_books.cbegin() + i));
    }

    return nullptr;
}

Book* LibraryService::getBook(const QUuid& uuid)
{
    for(size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).getUuid() == uuid)
            return &(*(m_books.begin() + i));
    }

    return nullptr;
}

int LibraryService::getBookIndex(const QUuid& uuid) const
{
    auto* book = getBook(uuid);
    if(book == nullptr)
        return -1;

    auto bookPosition = std::ranges::find_if(m_books,
                                             [&uuid](const Book& rhs)
                                             {
                                                 return rhs.getUuid() == uuid;
                                             });
    size_t index = bookPosition - m_books.begin();

    return index;
}

int LibraryService::getBookCount() const
{
    return m_books.size();
}

BookOperationStatus LibraryService::saveBookToFile(const QUuid& uuid,
                                                   const QString& pathToFolder)
{
    auto* book = getBook(uuid);
    if(book == nullptr)
    {
        qWarning() << QString("Saving book with uuid: %1 to folder %2 failed. "
                              " No book with this uuid exists.")
                          .arg(uuid.toString(), pathToFolder);
        return BookOperationStatus::BookDoesNotExist;
    }

    QString currentBookPath = book->getFilePath();
    QString destinaton = pathToFolder + "/" + QUrl(currentBookPath).fileName();

    auto result = QFile::copy(book->getFilePath(), destinaton);
    if(!result)
    {
        qWarning() << QString("Saving book with uuid: %1 to folder: %2 failed. "
                              "No book with this uuid exists.")
                          .arg(uuid.toString(), pathToFolder);

        return BookOperationStatus::OperationFailed;
    }


    return BookOperationStatus::Success;
}

bool LibraryService::refreshLastOpenedDateOfBook(const QUuid& uuid)
{
    auto* book = getBook(uuid);
    if(book == nullptr)
        return false;

    book->updateLastOpened();
    m_bookStorageManager->updateBook(*book);

    refreshUIForBook(uuid);
    return true;
}

void LibraryService::processDownloadedBook(const QUuid& uuid,
                                           const QString& filePath)
{
    auto* book = getBook(uuid);

    book->setFilePath(filePath);
    book->setDownloaded(true);

    // The book meta-data file does not exist locally, so create it
    m_bookStorageManager->addBookLocally(*book);

    refreshUIForBook(uuid);
}

void LibraryService::processDownloadedBookCover(const QUuid& uuid,
                                                const QString& filePath)
{
    auto* book = getBook(uuid);

    book->setCoverPath(filePath);
    book->setHasCover(true);
    m_bookStorageManager->updateBookLocally(*book);

    refreshUIWithNewCover(uuid, filePath);
}

void LibraryService::updateUsedBookStorage(long usedStorage,
                                           long bookStorageLimit)
{
    m_usedBookStorage = usedStorage;
    m_bookStorageLimit = bookStorageLimit;
}

void LibraryService::setupUserData(const QString& token, const QString& email)
{
    m_bookStorageManager->setUserData(email, token);

    loadLocalBooks();
    m_bookStorageManager->downloadRemoteBooks();

    m_fetchChangesTimer.start();
}

void LibraryService::loadLocalBooks()
{
    auto books = m_bookStorageManager->loadLocalBooks();
    for(auto book : books)
    {
        uninstallBookIfTheBookFileIsInvalid(book);
        addBookToLibrary(book);
    }
}

void LibraryService::uninstallBookIfTheBookFileIsInvalid(Book& book)
{
    // The file might have been moved or deleted from the user's filesystem,
    // from the last time the application was used. This would mean that the
    // underlying book file would be invalid (since it does not exist
    // anymore). If this happens, unsinstall the book, so that the user can
    // redownload it from the server.
    QFile bookFile(book.getFilePath());
    if(!bookFile.exists())
    {
        book.setFilePath("");
        book.setDownloaded(false);
    }
}

void LibraryService::clearUserData()
{
    m_bookStorageManager->clearUserData();
    m_fetchChangesTimer.stop();

    emit bookClearingStarted();
    m_books.clear();
    emit bookClearingEnded();
}

void LibraryService::updateLibrary(std::vector<Book>& books)
{
    // The remote library is the library fetched from the server and the
    // local library is the library on the client's PC. On startup we need
    // to make sure that both libraries are synchronized.
    mergeRemoteLibraryIntoLocalLibrary(books);
    mergeLocalLibraryIntoRemoteLibrary(books);
}

void LibraryService::mergeRemoteLibraryIntoLocalLibrary(
    std::vector<Book>& remoteBooks)
{
    for(auto& remoteBook : remoteBooks)
    {
        auto* localBook = getBook(remoteBook.getUuid());
        if(localBook != nullptr)
        {
            utility::BookMerger bookMerger;
            connect(&bookMerger, &utility::BookMerger::localBookCoverDeleted,
                    this, &LibraryService::refreshUIWithNewCover);
            connect(&bookMerger, &utility::BookMerger::bookDataChanged, this,
                    &LibraryService::refreshUIForBook);

            bookMerger.mergeBooks(*localBook, remoteBook, m_bookStorageManager);
            continue;
        }

        addBookToLibrary(remoteBook);

        // Get the cover for the remote book if it does not exist locally
        if(remoteBook.hasCover() && remoteBook.getCoverPath().isEmpty())
        {
            m_bookStorageManager->downloadBookCover(remoteBook.getUuid());
        }
    }
}

void LibraryService::mergeLocalLibraryIntoRemoteLibrary(
    const std::vector<Book>& remoteBooks)
{
    int bytesOfDataUploaded = 0;
    for(const auto& localBook : m_books)
    {
        bool localBookExistsOnServer = std::ranges::any_of(
            remoteBooks,
            [&localBook](const Book& remoteBook)
            {
                return remoteBook.getUuid() == localBook.getUuid();
            });

        // When the book was uploaded to the server at some point, and it does
        // not exist on the server anymore, it must have been deleted from
        // another device. Make sure to delete the book locally as well.
        if(!localBook.existsOnlyOnClient() && !localBookExistsOnServer)
        {
            deleteBookLocally(localBook);
            return;
        }

        // Ensure that we are not trying to upload the local books even
        // though we know that there is not enough space available. This would
        // just lead to annoying error messages.
        long bookSize = localBook.getSizeInBytes();
        long totalStorageSpace = m_usedBookStorage + bytesOfDataUploaded;
        bool enoughSpace = totalStorageSpace + bookSize < m_bookStorageLimit;
        if(!localBookExistsOnServer && enoughSpace)
        {
            m_bookStorageManager->addBook(localBook);
            bytesOfDataUploaded += bookSize;
        }
    }
}

void LibraryService::deleteBookLocally(const domain::entities::Book& book)
{
    utility::BookForDeletion bookToDelete {
        .uuid = book.getUuid(),
        .downloaded = book.isDownloaded(),
        .format = book.getFormat(),
    };

    auto bookPosition = getBookPosition(book.getUuid());
    int index = getBookIndex(book.getUuid());

    emit bookDeletionStarted(index);
    m_books.erase(bookPosition);
    emit bookDeletionEnded();

    m_bookStorageManager->deleteBookLocally(std::move(bookToDelete));
}

void LibraryService::refreshUIWithNewCover(const QUuid& uuid,
                                           const QString& path)
{
    auto book = getBook(uuid);
    auto index = getBookIndex(uuid);

    // To properly update the UI, we need to invalidate the current image
    // first and then set the new one, else Qt doesn't see the changes. We
    // do this by setting the path to an empty string first and then to the
    // actual path. We emit 'dataChanged' in between to update the UI.
    book->setCoverPath("");
    emit dataChanged(index);

    book->setCoverPath(path);
    emit dataChanged(index);
}

void LibraryService::refreshUIForBook(const QUuid& uuid)
{
    // The dataChanged signal invalidates the book model, which then
    // causes the UI to refresh and show the new changes.
    int index = getBookIndex(uuid);
    emit dataChanged(index);
}

}  // namespace application::services
