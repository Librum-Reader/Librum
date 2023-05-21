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
    // Loading books
    connect(m_bookStorageGateway,
            &IBookStorageGateway::gettingBooksMetaDataFinished, this,
            &BookStorageManager::processBookMetadata);

    // Save downloaded book
    connect(m_bookStorageGateway,
            &IBookStorageGateway::downloadingBookMediaFinished, this,
            &BookStorageManager::saveDownloadedBookMediaToFile);

    // Save book cover
    connect(m_bookStorageGateway,
            &IBookStorageGateway::downloadingBookCoverFinished, this,
            &BookStorageManager::saveDownloadedCoverToFile);
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

void BookStorageManager::saveDownloadedBookMediaToFile(const QByteArray& data,
                                                       const QUuid& uuid,
                                                       const QString& format)
{
    auto destDir = m_downloadedBooksTracker->getLibraryDir();
    QString fileName = uuid.toString(QUuid::WithoutBraces) + "." + format;
    auto destination = QUrl(destDir.filePath(fileName)).path();

    QFile file(destination);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Could not open new book file!";
        return;
    }

    file.write(data);
    emit finishedDownloadingBookMedia(uuid, destination);
}

void BookStorageManager::saveDownloadedCoverToFile(const QByteArray& data,
                                                   const QUuid& uuid)
{
    auto destDir = m_downloadedBooksTracker->getLibraryDir();
    QString fileName = getBookCoverPath(uuid);
    auto destination = QUrl(destDir.filePath(fileName)).path();

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

void BookStorageManager::processBookMetadata(std::vector<Book>& books)
{
    // Avoid storing books for logged out users by verifying login
    // status before adding books, else books might get loaded into
    // memory, even though the user is logged out.
    if(!userLoggedIn())
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

    emit loadingRemoteBooksFinished(books);
}

bool BookStorageManager::userLoggedIn()
{
    return !m_authenticationToken.isEmpty();
}

QString BookStorageManager::getBookCoverPath(const QUuid& uuid)
{
    auto dir = QDir(m_downloadedBooksTracker->getLibraryDir());
    auto fileName = QString("%1%2.%3").arg(m_bookCoverPrefix,
                                           uuid.toString(QUuid::WithoutBraces),
                                           m_bookCoverType);

    return dir.filePath(fileName);
}

void BookStorageManager::deleteBookFile(const QUuid& uuid,
                                        const QString& format)
{
    auto dir = QDir(m_downloadedBooksTracker->getLibraryDir());
    auto fileName =
        QString("%1.%2").arg(uuid.toString(QUuid::WithoutBraces), format);

    QFile bookFileToDelete(dir.filePath(fileName));
    bookFileToDelete.remove();
}

void BookStorageManager::addBook(const Book& bookToAdd)
{
    // Prevent adding remote books to the local library unless "downloaded" is
    // set to true.
    if(bookToAdd.isDownloaded())
        addBookLocally(bookToAdd);

    m_bookStorageGateway->createBook(m_authenticationToken, bookToAdd);
}

void BookStorageManager::addBookLocally(const domain::entities::Book& bookToAdd)
{
    m_downloadedBooksTracker->trackBook(bookToAdd);
}

void BookStorageManager::deleteBook(utility::BookForDeletion bookToDelete)
{
    // Non-downloaded books aren't in the local library, thus can't be untracked
    if(bookToDelete.downloaded)
    {
        m_downloadedBooksTracker->untrackBook(bookToDelete.uuid);
        deleteBookFile(bookToDelete.uuid, bookToDelete.format);
    }

    m_bookStorageGateway->deleteBook(m_authenticationToken, bookToDelete.uuid);
    deleteBookCoverLocally(bookToDelete.uuid);
}

void BookStorageManager::uninstallBook(const Book& book)
{
    m_downloadedBooksTracker->untrackBook(book.getUuid());
    deleteBookFile(book.getUuid(), book.getFormat());
}

void BookStorageManager::downloadBookMedia(const QUuid& uuid)
{
    m_bookStorageGateway->downloadBookMedia(m_authenticationToken, uuid);
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
    if(book.isDownloaded())
        m_downloadedBooksTracker->updateTrackedBook(book);
}

void BookStorageManager::updateBookRemotely(const domain::entities::Book& book)
{
    m_bookStorageGateway->updateBook(m_authenticationToken, book);
}

void BookStorageManager::updateBookCoverRemotely(const QUuid& uuid,
                                                 bool hasCover)
{
    if(hasCover)
        m_bookStorageGateway->changeBookCover(m_authenticationToken, uuid,
                                              getBookCoverPath(uuid));
    else
        m_bookStorageGateway->deleteBookCover(m_authenticationToken, uuid);
}

std::optional<QString> BookStorageManager::saveBookCoverToFile(
    const QUuid& uuid, const QPixmap& cover)
{
    QFile file(getBookCoverPath(uuid));
    if(!file.open(QFile::WriteOnly))
    {
        return std::nullopt;
    }

    int fileQuality = 20;
    cover.save(&file, m_bookCoverType.toStdString().c_str(), fileQuality);
    return file.fileName();
}

bool BookStorageManager::deleteBookCoverLocally(const QUuid& uuid)
{
    QFile file(getBookCoverPath(uuid));
    auto success = file.remove();

    return success;
}

void BookStorageManager::downloadBookCover(const QUuid& uuid)
{
    m_bookStorageGateway->getCoverForBook(m_authenticationToken, uuid);
}

bool BookStorageManager::bookCoverExistsLocally(const QUuid& uuid)
{
    QFile cover(getBookCoverPath(uuid));
    return cover.exists();
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