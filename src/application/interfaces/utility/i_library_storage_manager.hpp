#pragma once
#include <QObject>
#include <QPixmap>
#include <QString>
#include <QUuid>
#include <optional>
#include <vector>
#include "application_export.hpp"
#include "book.hpp"
#include "book_for_deletion.hpp"

namespace application
{

/**
 * The LibraryStorageManager manages the book storage for the local and remote
 * library.
 */
class APPLICATION_EXPORT ILibraryStorageManager : public QObject
{
    Q_OBJECT

public:
    virtual ~ILibraryStorageManager() noexcept = default;

    virtual void addBook(const domain::entities::Book& bookToAdd) = 0;
    virtual void addBookLocally(const domain::entities::Book& bookToAdd) = 0;
    virtual void deleteBook(utility::BookForDeletion bookToDelete) = 0;
    virtual void deleteAllBooks() = 0;
    virtual void deleteBookLocally(utility::BookForDeletion bookToDelete) = 0;
    virtual void uninstallBook(const domain::entities::Book& book) = 0;
    virtual void downloadBookMedia(const QUuid& uuid) = 0;
    virtual void updateBook(const domain::entities::Book& book) = 0;
    virtual void updateBookLocally(const domain::entities::Book& book) = 0;
    virtual void updateBookRemotely(const domain::entities::Book& book) = 0;
    virtual void updateBookCoverRemotely(const QUuid& uuid, bool hasCover) = 0;
    virtual QString saveBookCoverToFile(const QUuid& uuid,
                                        const QImage& cover) = 0;
    virtual bool deleteBookCoverLocally(const QUuid& uuid) = 0;
    virtual void downloadBookCover(const QUuid& uuid) = 0;
    virtual std::vector<domain::entities::Book> loadLocalBooks() = 0;
    virtual void downloadRemoteBooks() = 0;

    virtual void setUserData(const QString& email,
                             const QString& authToken) = 0;
    virtual void clearUserData() = 0;

signals:
    void finishedDownloadingRemoteBooks(
        std::vector<domain::entities::Book>& books);
    void downloadingBookMediaProgressChanged(const QUuid& uuid,
                                             qint64 bytesReceived,
                                             qint64 bytesTotal);
    void finishedDownloadingBookMedia(const QUuid& uuid,
                                      const QString& filePath);
    void finishedDownloadingBookCover(const QUuid& uuid,
                                      const QString& filePath);
    void storageLimitExceeded();
    void bookUploadSucceeded(const QUuid& uuid);
};

}  // namespace application
