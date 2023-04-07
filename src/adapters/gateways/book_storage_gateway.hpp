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
                    const domain::entities::Book& book) override;
    void deleteBook(const QString& authToken, const QUuid& uuid) override;
    void updateBook(const QString& authToken,
                    const domain::entities::Book& book) override;
    void changeBookCover(const QString& authToken, const QUuid& uuid,
                         const QString& path) override;
    void deleteBookCover(const QString& authToken, const QUuid& uuid) override;
    void getBooksMetaData(const QString& authToken) override;
    void downloadBook(const QString& authToken, const QUuid& uuid) override;

private slots:
    void proccessBooksMetadata(std::vector<QJsonObject>& jsonBooks);

private:
    enum class TagNamingStyle
    {
        ApiStyle,
        ClientStyle
    };

private:
    // The api and the client use different property names, convert as needed.
    // E.g. the client calls IDs "uuid"s where as the api calls them "guid"s
    void convertJsonBookToApiFormat(QJsonObject& jsonBook);
    QJsonArray renameTagProperties(const QJsonArray& tags,
                                   TagNamingStyle namingStyle);
    void renameJsonObjectKey(QJsonObject& jsonObject, const QString& oldKeyName,
                             const QString& newKeyName);

    IBookStorageAccess* m_bookStorageAccess;
};

}  // namespace adapters::gateways