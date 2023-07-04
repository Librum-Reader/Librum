#pragma once
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

private slots:
    void setBookCover(int bookId, const QImage& cover);

private:
    void saveBookToFile(const QByteArray& data);
    void saveBookMetaData(std::vector<domain::value_objects::FreeBook>& books);
    domain::value_objects::FreeBook* getFreeBookById(int id);
    int getFreeBookIndexById(int id);

    IFreeBooksStorageGateway* m_freeBooksStorageGateway;
    QString m_freeBooksFolderName = "downloaded_free_books";
    std::vector<domain::value_objects::FreeBook> m_freeBooks;

    static const int maxCoverWidth { 188 };
    static const int maxCoverHeight { 238 };
};

}  // namespace application::services
