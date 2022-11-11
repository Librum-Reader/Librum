#pragma once
#include <QPixmap>
#include <QTimer>
#include "book.hpp"
#include "i_book_metadata_helper.hpp"
#include "i_book_service.hpp"
#include "i_book_storage_gateway.hpp"
#include "i_downloaded_books_tracker.hpp"
#include "i_internet_connection_info.hpp"

namespace application::services
{

class BookService : public IBookService
{
    Q_OBJECT

public:
    BookService(IBookStorageGateway* bookStorageGateway,
                IBookMetadataHelper* bookMetadataHelper,
                IDownloadedBooksTracker* downloadedBooksTracker,
                IInternetConnectionInfo* internetConnectionInfo);

    BookOperationStatus addBook(const QString& filePath) override;
    BookOperationStatus deleteBook(const QUuid& uuid) override;
    BookOperationStatus uninstallBook(const QUuid& uuid) override;
    BookOperationStatus updateBook(
        const domain::models::Book& newBook) override;

    BookOperationStatus addTag(const QUuid& uuid,
                               const domain::models::Tag& tag) override;
    BookOperationStatus removeTag(const QUuid& uuid,
                                  const domain::models::Tag& tag) override;

    const std::vector<domain::models::Book>& getBooks() const override;
    const domain::models::Book* getBook(const QUuid& uuid) const override;
    domain::models::Book* getBook(const QUuid& uuid) override;
    int getBookIndex(const QUuid& uuid) const override;
    int getBookCount() const override;

    BookOperationStatus saveBookToPath(const QUuid& uuid,
                                       const QUrl& path) override;

public slots:
    bool refreshLastOpened(const QUuid& uuid) override;
    void setAuthenticationToken(const QString& token,
                                const QString& email) override;
    void clearAuthenticationToken() override;

private slots:
    void storeBookCover(const QPixmap* pixmap);
    void loadRemoteBooks();
    void addRemoteBooks(const std::vector<domain::models::Book>& books);

private:
    void loadBooks();
    void loadLocalBooks();
    void mergeBooks(domain::models::Book& original,
                    const domain::models::Book& toMerge);

    IBookStorageGateway* m_bookStorageGateway;
    IBookMetadataHelper* m_bookMetadataHelper;
    IDownloadedBooksTracker* m_downloadedBooksTracker;
    IInternetConnectionInfo* m_internetConnectionInfo;
    std::vector<domain::models::Book> m_books;
    QString m_authenticationToken;
    QString m_currentUserEmail;
};

}  // namespace application::services