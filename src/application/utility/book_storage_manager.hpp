#pragma once
#include <QString>
#include "i_book_storage_gateway.hpp"
#include "i_book_storage_manager.hpp"
#include "i_downloaded_books_tracker.hpp"

namespace application::utility
{

class BookStorageManager : public IBookStorageManager
{
public:
    BookStorageManager(IBookStorageGateway* bookStorageGateway,
                       IDownloadedBooksTracker* downloadedBooksTracker);

    void addBook(const domain::entities::Book& bookToAdd) override;
    void addBookLocally(const domain::entities::Book& bookToAdd) override;
    void deleteBook(BookForDeletion bookToDelete) override;
    void deleteBookLocally(QUuid uuid) override;
    void uninstallBook(const QUuid& uuid) override;
    void downloadBook(const QUuid& uuid) override;
    void updateBook(const domain::entities::Book& book) override;
    void updateBookLocally(const domain::entities::Book& book) override;
    void updateBookRemotely(const domain::entities::Book& book) override;
    void updateBookCoverRemotely(const QUuid& uuid, bool hasCover) override;
    std::optional<QString> saveBookCoverToFile(const QUuid& uuid,
                                               const QPixmap& cover) override;
    bool deleteBookCoverLocally(const QUuid& uuid) override;
    void downloadBookCover(const QUuid& uuid) override;
    std::vector<domain::entities::Book> loadLocalBooks() override;
    void loadRemoteBooks() override;

    void setUserData(const QString& email, const QString& authToken) override;
    void clearUserData() override;

private slots:
    void saveDownloadedBookToFile(const QByteArray& data, const QUuid& uuid,
                                  const QString& format);
    void saveDownloadedCoverToFile(const QByteArray& data, const QUuid& uuid);
    void processBookMetadata(std::vector<domain::entities::Book>& books);

private:
    bool userLoggedIn();
    QString getBookCoverPath(const QUuid& uuid);

    QString m_bookCoverPrefix = "cover_";
    QString m_bookCoverType = "png";
    IBookStorageGateway* m_bookStorageGateway;
    IDownloadedBooksTracker* m_downloadedBooksTracker;
    QString m_authenticationToken;
};

}  // namespace application::utility