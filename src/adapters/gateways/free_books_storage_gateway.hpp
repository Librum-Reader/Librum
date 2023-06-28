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

private:
    IFreeBooksStorageAccess* m_freeBooksStorageAccess;

    void proccessBooksMetadata(const QByteArray& data);
    void assignValuesToBook(domain::entities::FreeBook& book,
                            const QJsonObject& values);
    void addAuthorsToBook(domain::entities::FreeBook& book,
                          const QJsonArray& authors);
    void addLanguagesToBook(domain::entities::FreeBook& book,
                            const QJsonArray& languages);
    void addFormatsToBook(domain::entities::FreeBook& book,
                          const QJsonArray& formats);
};

}  // namespace adapters::gateways
