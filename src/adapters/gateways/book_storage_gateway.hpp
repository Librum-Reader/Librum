#pragma once
#include <QJsonObject>
#include <QObject>
#include "i_book_storage_access.hpp"
#include "i_book_storage_gateway.hpp"

namespace adapters::gateways
{

class BookStorageGateway : public application::IBookStorageGateway
{
    Q_OBJECT

public:
    BookStorageGateway(IBookStorageAccess* bookStorageAccess);

    void createBook(const QString& authToken,
                    const domain::models::Book& book) override;
    void deleteBook(const QString& authToken, const QUuid& uuid) override;
    void updateBook(const QString& authToken,
                    const domain::models::Book& book) override;
    void getBooksMetaData(const QString& authToken) override;
    void downloadBook(const QString& authToken, const QUuid& uuid) override;

private slots:
    void proccessBooksMetadata(std::vector<QJsonObject>& jsonBooks);

private:
    void renameJsonObjectKey(QJsonObject& jsonObject, const QString& oldKeyName,
                             const QString& newKeyName);

private:
    IBookStorageAccess* m_bookStorageAccess;
};

}  // namespace adapters::gateways