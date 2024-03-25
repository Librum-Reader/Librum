#include "library_storage_manager.hpp"
#include <vector>
#include "save_book_helper.hpp"

using namespace domain::entities;
using application::utility::BookForDeletion;

namespace application::managers
{

LibraryStorageManager::LibraryStorageManager(
    ILibraryStorageGateway* bookStorageGateway,
    ILocalLibraryTracker* downloadedBooksTracker) :
    m_bookStorageGateway(bookStorageGateway),
    m_downloadedBooksTracker(downloadedBooksTracker)
{
    // Loading books
    connect(m_bookStorageGateway,
            &ILibraryStorageGateway::gettingBooksMetaDataFinished, this,
            &LibraryStorageManager::processBookMetadata);

    // Save downloaded book
    connect(m_bookStorageGateway,
            &ILibraryStorageGateway::downloadingBookMediaChunkReady, this,
            &LibraryStorageManager::saveDownloadedBookMediaChunkToFile);

    // Downloading book media progress
    connect(m_bookStorageGateway,
            &ILibraryStorageGateway::downloadingBookMediaProgressChanged, this,
            &LibraryStorageManager::downloadingBookMediaProgressChanged);

    // Save book cover
    connect(m_bookStorageGateway,
            &ILibraryStorageGateway::downloadingBookCoverFinished, this,
            &LibraryStorageManager::saveDownloadedCoverToFile);

    // Storage limit exceeded
    connect(m_bookStorageGateway, &ILibraryStorageGateway::storageLimitExceeded,
            this, &LibraryStorageManager::storageLimitExceeded);

    // Book upload succeeded
    connect(m_bookStorageGateway, &ILibraryStorageGateway::bookUploadSucceeded,
            this, &LibraryStorageManager::bookUploadSucceeded);
}

void LibraryStorageManager::setUserData(const QString& email,
                                        const QString& authToken)
{
    m_authenticationToken = authToken;
    m_downloadedBooksTracker->setLibraryOwner(email);
}

void LibraryStorageManager::clearUserData()
{
    m_authenticationToken.clear();
    m_downloadedBooksTracker->clearLibraryOwner();
}

void LibraryStorageManager::saveDownloadedBookMediaChunkToFile(
    const QByteArray& data, bool isLastChunk, const QUuid& uuid,
    const QString& extension)
{
    auto destDir = m_downloadedBooksTracker->getLibraryDir();
    QString fileName = uuid.toString(QUuid::WithoutBraces) + "." + extension;
    auto destination = destDir.filePath(fileName);

    application::utility::saveDownloadedBookMediaChunkToFile(
        data, isLastChunk, fileName, destination);

    if(isLastChunk)
    {
        emit finishedDownloadingBookMedia(uuid, destination);
    }
}

void LibraryStorageManager::saveDownloadedCoverToFile(const QByteArray& data,
                                                      const QUuid& uuid)
{
    QString destination = getBookCoverPath(uuid);

    QFile file(destination);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Could not open new cover file!";
        return;
    }

    file.write(data);

    // Manually close to make sure the data is written to file before continuing
    file.close();

    emit finishedDownloadingBookCover(uuid, destination);
}

void LibraryStorageManager::processBookMetadata(std::vector<Book>& books,
                                                bool success)
{
    // Avoid storing books for logged out users by verifying login
    // status before adding books, else books might get loaded into
    // memory, even though the user is logged out.
    if(!userLoggedIn() || !success)
        return;


    // Set the cover paths for the remote books
    for(auto& book : books)
    {
        if(book.hasCover() && bookCoverExistsLocally(book.getUuid()))
        {
            auto& uuid = book.getUuid();
            book.setCoverPath(getBookCoverPath(uuid));
        }
    }

    emit finishedDownloadingRemoteBooks(books);
}

bool LibraryStorageManager::userLoggedIn()
{
    return !m_authenticationToken.isEmpty();
}

QString LibraryStorageManager::getBookCoverPath(const QUuid& uuid)
{
    auto dir = QDir(m_downloadedBooksTracker->getLibraryDir());
    auto fileName = QString("%1%2.%3").arg(m_bookCoverPrefix,
                                           uuid.toString(QUuid::WithoutBraces),
                                           m_bookCoverType);

    return dir.filePath(fileName);
}

void LibraryStorageManager::deleteBookFile(const QUuid& uuid,
                                           const QString& extension)
{
    auto dir = QDir(m_downloadedBooksTracker->getLibraryDir());
    auto fileName =
        QString("%1.%2").arg(uuid.toString(QUuid::WithoutBraces), extension);

    QFile bookFileToDelete(dir.filePath(fileName));
    bookFileToDelete.remove();
}

void LibraryStorageManager::addBook(const Book& bookToAdd)
{
    // Prevent adding remote books to the local library unless "downloaded" is
    // set to true.
    if(bookToAdd.isDownloaded())
        addBookLocally(bookToAdd);

    m_bookStorageGateway->createBook(m_authenticationToken, bookToAdd);
}

void LibraryStorageManager::addBookLocally(
    const domain::entities::Book& bookToAdd)
{
    m_downloadedBooksTracker->trackBook(bookToAdd);
}

void LibraryStorageManager::deleteBook(utility::BookForDeletion bookToDelete)
{
    // Non-downloaded books aren't in the local library, thus can't be untracked
    if(bookToDelete.downloaded)
        deleteBookLocally(bookToDelete);

    m_bookStorageGateway->deleteBook(m_authenticationToken, bookToDelete.uuid);
    deleteBookCoverLocally(bookToDelete.uuid);
}

void LibraryStorageManager::deleteAllBooks()
{
    QDir localLibraryDir(m_downloadedBooksTracker->getLibraryDir());

    localLibraryDir.removeRecursively();
}

void LibraryStorageManager::deleteBookLocally(BookForDeletion bookToDelete)
{
    m_downloadedBooksTracker->untrackBook(bookToDelete.uuid);
    deleteBookFile(bookToDelete.uuid, bookToDelete.extension);
    deleteBookCoverLocally(bookToDelete.uuid);
}

void LibraryStorageManager::uninstallBook(const Book& book)
{
    m_downloadedBooksTracker->untrackBook(book.getUuid());
    deleteBookFile(book.getUuid(), book.getExtension());
}

void LibraryStorageManager::downloadBookMedia(const QUuid& uuid)
{
    m_bookStorageGateway->downloadBookMedia(m_authenticationToken, uuid);
}

void LibraryStorageManager::updateBook(const Book& book)
{
    updateBookLocally(book);
    updateBookRemotely(book);
}

void LibraryStorageManager::updateBookLocally(
    const domain::entities::Book& book)
{
    // Prevent updating remote books in the local library unless the book is
    // downloaded. If its not downloaded, there is no local file to update.
    if(book.isDownloaded())
        m_downloadedBooksTracker->updateTrackedBook(book);
}

void LibraryStorageManager::updateBookRemotely(
    const domain::entities::Book& book)
{
    m_bookStorageGateway->updateBook(m_authenticationToken, book);
}

void LibraryStorageManager::updateBookCoverRemotely(const QUuid& uuid,
                                                    bool hasCover)
{
    if(hasCover)
        m_bookStorageGateway->changeBookCover(m_authenticationToken, uuid,
                                              getBookCoverPath(uuid));
    else
        m_bookStorageGateway->deleteBookCover(m_authenticationToken, uuid);
}

QString LibraryStorageManager::saveBookCoverToFile(const QUuid& uuid,
                                                   const QImage& cover)
{
    QFile file(getBookCoverPath(uuid));
    if(!file.open(QFile::WriteOnly))
        return "";

    int fileQuality = 20;
    cover.save(&file, m_bookCoverType.toStdString().c_str(), fileQuality);
    return file.fileName();
}

bool LibraryStorageManager::deleteBookCoverLocally(const QUuid& uuid)
{
    QFile file(getBookCoverPath(uuid));
    auto success = file.remove();

    return success;
}

void LibraryStorageManager::downloadBookCover(const QUuid& uuid)
{
    m_bookStorageGateway->getCoverForBook(m_authenticationToken, uuid);
}

bool LibraryStorageManager::bookCoverExistsLocally(const QUuid& uuid)
{
    QFile cover(getBookCoverPath(uuid));
    return cover.exists();
}

std::vector<Book> LibraryStorageManager::loadLocalBooks()
{
    auto m_localBooks = m_downloadedBooksTracker->getTrackedBooks();
    return m_localBooks;
}

void LibraryStorageManager::downloadRemoteBooks()
{
    m_bookStorageGateway->getBooksMetaData(m_authenticationToken);
}

}  // namespace application::managers
