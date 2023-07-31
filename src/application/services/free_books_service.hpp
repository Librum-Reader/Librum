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

    void getBooksMetadata(const QString& author, const QString& title) override;
    void getBookMedia(const QString& url) override;
    std::vector<domain::value_objects::FreeBook>& getFreeBooks() override;

public slots:
    void setupUserData(const QString& token, const QString& email) override;
    void clearUserData() override;

private slots:
    void setBookCover(int bookId, const QImage& cover);
    void saveDownloadedBookMediaChunkToFile(const QByteArray& data,
                                            bool isLastChunk, const QUuid& uuid,
                                            const QString& format);

private:
    void saveBookMetaData(std::vector<domain::value_objects::FreeBook>& books);
    domain::value_objects::FreeBook* getFreeBookById(int id);
    int getFreeBookIndexById(int id);
    QDir getLibraryDir();

    IFreeBooksStorageGateway* m_freeBooksStorageGateway;
    std::vector<domain::value_objects::FreeBook> m_freeBooks;

    QString m_userEmail;
    static const int maxCoverWidth { 188 };
    static const int maxCoverHeight { 238 };
};

}  // namespace application::services
