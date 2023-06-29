#pragma once
#include "i_free_books_service.hpp"
#include "i_free_books_storage_gateway.hpp"

namespace application::services
{

class FreeBooksService : public IFreeBooksService
{
    Q_OBJECT

public:
    FreeBooksService(IFreeBooksStorageGateway* freeBooksStorageGateway);

    void getBooksMetadata() override;
    void getBookMedia(const QString& url) override;

private:
    IFreeBooksStorageGateway* m_freeBooksStorageGateway;
    QString m_freeBooksFolderName = "downloaded_free_books";

    void saveBookToFile(const QByteArray& data);
};

}  // namespace application::services
