#pragma once
#include <QDir>
#include <vector>
#include "free_book.hpp"
#include "i_free_books_service.hpp"
#include "i_free_books_storage_gateway.hpp"

namespace application::services
{

class FreeBooksService : public IFreeBooksService
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

public slots:
    void fetchBooksMetadataPage(const QString& url) override;
    void setupUserData(const QString& token, const QString& email) override;
    void clearUserData() override;

private slots:
    void setBookCover(int id, const QImage& cover);
    void saveDownloadedBookMediaChunkToFile(const QByteArray& data,
                                            bool isLastChunk, const QUuid& uuid,
                                            const QString& format);

private:
    void saveBookMetaData(std::vector<domain::value_objects::FreeBook>& books,
                          const int booksTotalCount,
                          const QString& nextMetadataPageUrl,
                          const QString& prevMetadataPageUrl);
    void setMediaDownloadProgressForBook(const int id, qint64 bytesReceived,
                                         qint64 bytesTotal);
    domain::value_objects::FreeBook* getFreeBookById(int id);
    int getFreeBookIndexById(int id);
    QDir getLibraryDir() const;
    QString getUserLibraryName(const QString& email) const;

    IFreeBooksStorageGateway* m_freeBooksStorageGateway;
    std::vector<domain::value_objects::FreeBook> m_freeBooks;

    QString m_userEmail;
    static const int maxCoverWidth { 188 };
    static const int maxCoverHeight { 238 };
};

}  // namespace application::services
