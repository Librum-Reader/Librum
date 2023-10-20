#pragma once
#include <QDir>
#include <set>
#include <vector>
#include "application_export.hpp"
#include "free_book.hpp"
#include "i_free_books_service.hpp"
#include "i_free_books_storage_gateway.hpp"

namespace application::services
{

class APPLICATION_EXPORT FreeBooksService : public IFreeBooksService
{
    Q_OBJECT

public:
    FreeBooksService(IFreeBooksStorageGateway* freeBooksStorageGateway);

    void fetchFirstBooksMetadataPageWithFilter(
        const QString& authorsAndTitle) override;
    void getBookMedia(const int id, const QString& url) override;
    void getBookCover(const int id) override;
    void deleteBookCover(const int id) override;
    std::vector<domain::value_objects::FreeBook>& getFreeBooks() override;
    void deleteAllBooks() override;
    bool isBookDownloaded(int id) override;
    void proccessDownloadedBookIds(const std::set<int>& newIds) override;
    void markBookAsDownloaded(int id) override;
    void unmarkBookAsDownloaded(int id) override;

public slots:
    void fetchBooksMetadataPage(const QString& url) override;
    void setupUserData(const QString& token, const QString& email) override;
    void clearUserData() override;

private slots:
    void setBookCover(int id, const QImage& cover);
    void saveDownloadedBookMediaChunkToFile(int id, const QUuid& uuid,
                                            const QByteArray& data,
                                            const QString& format,
                                            bool isLastChunk);

private:
    void processBookMetadata(
        std::vector<domain::value_objects::FreeBook>& books,
        const int booksTotalCount, const QString& nextMetadataPageUrl,
        const QString& prevMetadataPageUrl);
    void setMediaDownloadProgressForBook(const int id, qint64 bytesReceived,
                                         qint64 bytesTotal);
    domain::value_objects::FreeBook* getFreeBookById(int id);
    int getFreeBookIndexById(int id);
    QDir getLibraryDir() const;
    QString getUserLibraryName(const QString& email) const;

    IFreeBooksStorageGateway* m_freeBooksStorageGateway;
    std::vector<domain::value_objects::FreeBook> m_freeBooks;

    // Use std::set for quicker searching
    std::set<int> m_downloadedFreeBookIds;
    static const int invalidFreeBookId = 0;

    QString m_userEmail;
    static const int maxCoverWidth { 188 };
    static const int maxCoverHeight { 238 };
};

}  // namespace application::services
