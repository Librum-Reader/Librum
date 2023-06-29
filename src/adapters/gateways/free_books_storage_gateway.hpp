#pragma once
#include <QJsonArray>
#include <QJsonObject>
#include "i_free_books_storage_access.hpp"
#include "i_free_books_storage_gateway.hpp"

namespace adapters::gateways
{

class FreeBooksStorageGateway : public application::IFreeBooksStorageGateway
{
    Q_OBJECT

public:
    FreeBooksStorageGateway(IFreeBooksStorageAccess* freeBooksStorageAccess);

    void getBooksMetadata() override;
    void getBookMedia(const QString& url) override;

private:
    IFreeBooksStorageAccess* m_freeBooksStorageAccess;

    void proccessBooksMetadata(const QByteArray& data);
    void assignValuesToBook(domain::value_objects::FreeBook& book,
                            const QJsonObject& values);
    void addAuthorsToBook(domain::value_objects::FreeBook& book,
                          const QJsonArray& authors);
    void addLanguagesToBook(domain::value_objects::FreeBook& book,
                            const QJsonArray& languages);
    void addFormatsToBook(domain::value_objects::FreeBook& book,
                          const QJsonArray& formats);
    void getCoverForBook(const QJsonObject& book);
    void proccessBookCover(int bookId, const QByteArray& data);
};

}  // namespace adapters::gateways
