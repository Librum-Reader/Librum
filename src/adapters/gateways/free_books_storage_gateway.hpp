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
    domain::value_objects::FreeBook getBookFromJson(const QJsonObject& values);
    QString formatAuthors(QJsonArray authors);
    QString formatLanguages(QJsonArray languages);
    void getCoverForBook(const QJsonObject& book);
    void proccessBookCover(int bookId, const QByteArray& data);
};

}  // namespace adapters::gateways
