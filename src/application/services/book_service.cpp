#include "book_service.hpp"
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QPixmap>
#include <QTime>
#include <ranges>
#include "book_for_deletion.hpp"
#include "book_merger.hpp"
#include "book_operation_status.hpp"
#include "i_book_metadata_helper.hpp"
#include "qfileinfo.h"

using namespace domain::entities;
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

    // Load the cover after creating book to avoid adding to a non-existent book
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

    m_bookStorageManager->uninstallBook(*book);
    book->setDownloaded(false);

    refreshUIForBook(uuid);
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

    if(!newBook.isValid())
    {
        qWarning() << QString("Failed updating book with uuid: %1."
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

    // An empty file path indicates that the book cover shall be deleted.
    if(filePath.isEmpty())
    {
        deleteBookCover(*book);
    }
    else
    {
        auto absoluteFilePath = QUrl(filePath).path();
        auto success = setNewBookCover(*book, absoluteFilePath);
        if(!success)
            return BookOperationStatus::OperationFailed;
    }

    m_bookStorageManager->updateBook(*book);
    m_bookStorageManager->updateBookCoverRemotely(book->getUuid(),
                                                  book->hasCover());
    return BookOperationStatus::Success;
}

void BookService::deleteBookCover(Book& book)
{
    auto success = m_bookStorageManager->deleteBookCoverLocally(book.getUuid());
    if(!success)
    {
        qWarning() << QString("Failed deleting the local book cover for book "
                              "with uuid: %1."
                              "Deleting the file failed.")
                          .arg(book.getUuid().toString(QUuid::WithoutBraces));
    }

    book.setCoverPath("");
    book.setHasCover(false);
    book.updateCoverLastModified();

    refreshUIForBook(book.getUuid());
}

bool BookService::setNewBookCover(Book& book, QString filePath)
{
    auto uuid = book.getUuid();
    QFileInfo newCoverFile(filePath);
    if(!newCoverFile.exists() || !newCoverFile.isFile())
    {
        qWarning() << QString("Failed setting new cover for book with uuid: %1."
                              "The given file at: %2 is invalid.")
                          .arg(uuid.toString(), filePath);
        return false;
    }

    QPixmap newCover(filePath);
    if(newCover.isNull())
    {
        qWarning() << QString("Failed setting new cover for book with uuid: %1."
                              "Can't open new cover image at: %2.")
                          .arg(uuid.toString(), filePath);
        return false;
    }

    // Scale the book cover to the correct size
    newCover = newCover.scaled(Book::maxCoverWidth, Book::maxCoverHeight,
                               Qt::KeepAspectRatio, Qt::SmoothTransformation);

    auto path = m_bookStorageManager->saveBookCoverToFile(uuid, newCover);
    if(!path.has_value())
    {
        qWarning() << QString("Failed setting new cover for book with uuid: %1."
                              "Saving new cover image failed.")
                          .arg(uuid.toString(), filePath);
        return false;
    }

    book.setHasCover(true);
    book.updateCoverLastModified();

    refreshUIWithNewCover(uuid, path.value());
    return true;
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

    book->updateLastOpened();
    m_bookStorageManager->updateBook(*book);

    refreshUIForBook(uuid);
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

    refreshUIForBook(uuid);
}

void BookService::processDownloadedBookCover(const QUuid& uuid,
                                             const QString& filePath)
{
    auto* book = getBook(uuid);

    book->setCoverPath(filePath);
    book->setHasCover(true);
    m_bookStorageManager->updateBookLocally(*book);

    refreshUIWithNewCover(uuid, filePath);
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
        uninstallBookIfTheBookFileIsInvalid(book);

        emit bookInsertionStarted(m_books.size());
        m_books.emplace_back(book);
        emit bookInsertionEnded();
    }
}

void BookService::uninstallBookIfTheBookFileIsInvalid(Book& book)
{
    // The file might have been moved or deleted from the user's filesystem,
    // from the last time the application was used. This would mean that the
    // underlying book file would be invalid (since it does not exist
    // anymore). If this happens, unsinstall the book, so that the user can
    // redownload it from the server.
    QFile bookFile(QUrl(book.getFilePath()).path());
    if(!bookFile.exists())
    {
        book.setFilePath("");
        book.setDownloaded(false);
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
            utility::BookMerger bookMerger;
            connect(&bookMerger, &utility::BookMerger::localBookCoverDeleted,
                    this, &BookService::refreshUIWithNewCover);
            connect(&bookMerger, &utility::BookMerger::bookDataChanged, this,
                    &BookService::refreshUIForBook);

            bookMerger.mergeBooks(*localBook, remoteBook, m_bookStorageManager);
            continue;
        }

        // Add the remote book to the local library if it does not exist
        emit bookInsertionStarted(m_books.size());
        m_books.emplace_back(remoteBook);
        emit bookInsertionEnded();


        // Get the cover for the remote book if it does not exist locally
        if(remoteBook.hasCover() && remoteBook.getCoverPath().isEmpty())
        {
            m_bookStorageManager->downloadBookCover(remoteBook.getUuid());
        }
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

void BookService::refreshUIWithNewCover(const QUuid& uuid, const QString& path)
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

void BookService::refreshUIForBook(const QUuid& uuid)
{
    // The dataChanged signal invalidates the book model, which then
    // causes the UI to refresh and show the new changes.
    int index = getBookIndex(uuid);
    emit dataChanged(index);
}

}  // namespace application::services
