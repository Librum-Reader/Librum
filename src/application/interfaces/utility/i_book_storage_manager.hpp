#pragma once
#include <QObject>
#include <QPixmap>
#include <QString>
#include <QUuid>
#include <optional>
#include <vector>
#include "book.hpp"
#include "book_for_deletion.hpp"

namespace application
{

/**
 * The BookStorageManager manages the book storage for the local and remote
 * library.
 */
class IBookStorageManager : public QObject
{
    Q_OBJECT

public:
    virtual ~IBookStorageManager() noexcept = default;

    virtual void addBook(const domain::entities::Book& bookToAdd) = 0;
    virtual void addBookLocally(const domain::entities::Book& bookToAdd) = 0;
    virtual void deleteBook(utility::BookForDeletion bookToDelete) = 0;
    virtual void deleteBookLocally(QUuid uuid) = 0;
    virtual void uninstallBook(const QUuid& uuid) = 0;
    virtual void downloadBook(const QUuid& uuid) = 0;
    virtual void updateBook(const domain::entities::Book& book) = 0;
    virtual void updateBookLocally(const domain::entities::Book& book) = 0;
    virtual void updateBookRemotely(const domain::entities::Book& book) = 0;
    virtual std::optional<QString> saveBookCoverToFile(
        const QUuid& uuid, const QPixmap& cover) = 0;
    virtual bool deleteBookCover(const QUuid& uuid) = 0;
    virtual std::vector<domain::entities::Book> loadLocalBooks() = 0;
    virtual void loadRemoteBooks() = 0;

    virtual void setUserData(const QString& email,
                             const QString& authToken) = 0;
    virtual void clearUserData() = 0;

signals:
    void loadingRemoteBooksFinished(
        const std::vector<domain::entities::Book>& books);
    void finishedDownloadingBook(const QUuid& uuid, const QString& filePath);
};

}  // namespace application