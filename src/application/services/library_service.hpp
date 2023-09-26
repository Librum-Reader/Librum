#pragma once
#include <QImage>
#include <QTimer>
#include <utility>
#include "application_export.hpp"
#include "book.hpp"
#include "i_library_service.hpp"
#include "i_library_storage_manager.hpp"
#include "i_metadata_extractor.hpp"

namespace application::services
{

class APPLICATION_EXPORT LibraryService : public ILibraryService
{
    Q_OBJECT

public:
    LibraryService(IMetadataExtractor* bookMetadataHelper,
                   ILibraryStorageManager* bookStorageManager);

    void downloadBooks() override;
    BookOperationStatus addBook(const QString& filePath) override;
    BookOperationStatus deleteBook(const QUuid& uuid) override;
    BookOperationStatus deleteAllBooks() override;
    BookOperationStatus uninstallBook(const QUuid& uuid) override;
    BookOperationStatus downloadBookMedia(const QUuid& uuid) override;
    BookOperationStatus updateBook(
        const domain::entities::Book& newBook) override;
    BookOperationStatus changeBookCover(const QUuid& uuid,
                                        const QString& filePath) override;
    BookOperationStatus saveBookToFile(const QUuid& uuid,
                                       const QString& path) override;

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
    void updateUsedBookStorage(long usedStorage,
                               long bookStorageLimit) override;

private slots:
    void updateLibrary(std::vector<domain::entities::Book>& books);
    void processDownloadedBook(const QUuid& uuid, const QString& filePath);
    void processDownloadedBookCover(const QUuid& uuid, const QString& filePath);
    void refreshUIWithNewCover(const QUuid& uuid, const QString& path);
    void refreshUIForBook(const QUuid& uuid);

private:
    auto getBookPosition(const QUuid& uuid);
    void loadLocalBooks();
    void uninstallBookIfTheBookFileIsInvalid(domain::entities::Book& book);
    void mergeRemoteLibraryIntoLocalLibrary(
        std::vector<domain::entities::Book>& remoteBooks);
    void mergeLocalLibraryIntoRemoteLibrary(
        const std::vector<domain::entities::Book>& remoteBooks);
    void deleteBookCover(domain::entities::Book& book);
    bool setNewBookCover(domain::entities::Book& book, QString filePath);
    void addBookToLibrary(const domain::entities::Book& book);
    void setMediaDownloadProgressForBook(const QUuid& uuid,
                                         qint64 bytesReceived,
                                         qint64 bytesTotal);
    void deleteBookLocally(const domain::entities::Book& book);

    IMetadataExtractor* m_bookMetadataHelper;
    ILibraryStorageManager* m_bookStorageManager;
    std::vector<domain::entities::Book> m_books;
    long m_usedBookStorage = 0;
    long m_bookStorageLimit = 0;
    QTimer m_fetchChangesTimer;
    const int m_fetchChangedInterval = 30'000;
};

}  // namespace application::services
