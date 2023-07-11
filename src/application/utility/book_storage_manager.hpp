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
    void deleteBookLocally(BookForDeletion bookToDelete) override;
    void uninstallBook(const domain::entities::Book& book) override;
    void downloadBookMedia(const QUuid& uuid) override;
    void updateBook(const domain::entities::Book& book) override;
    void updateBookLocally(const domain::entities::Book& book) override;
    void updateBookRemotely(const domain::entities::Book& book) override;
    void updateBookCoverRemotely(const QUuid& uuid, bool hasCover) override;
    QString saveBookCoverToFile(const QUuid& uuid,
                                const QImage& cover) override;
    bool deleteBookCoverLocally(const QUuid& uuid) override;
    void downloadBookCover(const QUuid& uuid) override;
    std::vector<domain::entities::Book> loadLocalBooks() override;
    void downloadRemoteBooks() override;

    void setUserData(const QString& email, const QString& authToken) override;
    void clearUserData() override;

private slots:
    void saveDownloadedBookMediaChunkToFile(const QByteArray& data,
                                            bool isLastChunk, const QUuid& uuid,
                                            const QString& format);
    void saveDownloadedCoverToFile(const QByteArray& data, const QUuid& uuid);
    void processBookMetadata(std::vector<domain::entities::Book>& books);

private:
    bool userLoggedIn();
    void deleteBookFile(const QUuid& uuid, const QString& format);
    QString getBookCoverPath(const QUuid& uuid);
    bool bookCoverExistsLocally(const QUuid& uuid);

    QString m_bookCoverPrefix = "cover_";
    QString m_bookCoverType = "png";
    IBookStorageGateway* m_bookStorageGateway;
    IDownloadedBooksTracker* m_downloadedBooksTracker;
    QString m_authenticationToken;
};

}  // namespace application::utility
