#pragma once
#include <QPixmap>
#include <QTimer>
#include <utility>
#include "book.hpp"
#include "i_book_metadata_helper.hpp"
#include "i_book_service.hpp"
#include "i_book_storage_manager.hpp"
#include "merge_status.hpp"

namespace application::services
{

class BookService : public IBookService
{
    Q_OBJECT

public:
    BookService(IBookMetadataHelper* bookMetadataHelper,
                IBookStorageManager* bookStorageManager);

    BookOperationStatus addBook(const QString& filePath) override;
    BookOperationStatus deleteBook(const QUuid& uuid) override;
    BookOperationStatus uninstallBook(const QUuid& uuid) override;
    BookOperationStatus downloadBook(const QUuid& uuid) override;
    BookOperationStatus updateBook(
        const domain::entities::Book& newBook) override;
    BookOperationStatus changeBookCover(const QUuid& uuid,
                                        const QString& filePath) override;
    BookOperationStatus saveBookToFile(const QUuid& uuid,
                                       const QUrl& path) override;

    BookOperationStatus addTagToBook(const QUuid& uuid,
                                     const domain::entities::Tag& tag) override;
    BookOperationStatus removeTagFromBook(const QUuid& bookUuid,
                                          const QUuid& tagUuid) override;
    BookOperationStatus renameTagOfBook(const QUuid& bookUuid,
                                        const QUuid& tagUuid,
                                        const QString& newName) override;

    const std::vector<domain::entities::Book>& getBooks() const override;
    const domain::entities::Book* getBook(const QUuid& uuid) const override;
    domain::entities::Book* getBook(const QUuid& uuid) override;
    int getBookIndex(const QUuid& uuid) const override;
    int getBookCount() const override;

public slots:
    bool refreshLastOpenedDateOfBook(const QUuid& uuid) override;
    void setupUserData(const QString& token, const QString& email) override;
    void clearUserData() override;

private slots:
    void processBookCover(const QPixmap* pixmap);
    void updateLibrary(const std::vector<domain::entities::Book>& books);
    void processDownloadedBook(const QUuid& uuid, const QString& filePath);

private:
    auto getBookPosition(const QUuid& uuid);
    void loadLocalBooks();
    void checkIfBookFileStillExists(domain::entities::Book& book);
    void mergeRemoteLibraryIntoLocalLibrary(
        const std::vector<domain::entities::Book>& remoteBooks);
    void mergeLocalLibraryIntoRemoteLibrary(
        const std::vector<domain::entities::Book>& remoteBooks);
    void mergeBooks(domain::entities::Book& original,
                    const domain::entities::Book& toMerge);
    utility::MergeStatus mergeCurrentPage(domain::entities::Book& original,
                                          const domain::entities::Book& mergee);
    utility::MergeStatus mergeBookData(domain::entities::Book& original,
                                       const domain::entities::Book& mergee);

    IBookMetadataHelper* m_bookMetadataHelper;
    IBookStorageManager* m_bookStorageManager;
    std::vector<domain::entities::Book> m_books;
    QTimer m_fetchChangesTimer;
    const int m_fetchChangedInterval = 15'000;
};

}  // namespace application::services