#pragma once
#include <QJsonObject>
#include <QObject>
#include "adapters_export.hpp"
#include "i_library_storage_access.hpp"
#include "i_library_storage_gateway.hpp"

namespace adapters::gateways
{

class ADAPTERS_EXPORT LibraryStorageGateway
    : public application::ILibraryStorageGateway
{
    Q_OBJECT

public:
    LibraryStorageGateway(ILibraryStorageAccess* bookStorageAccess);

    void createBook(const QString& authToken,
                    const domain::entities::Book& book) override;
    void deleteBook(const QString& authToken, const QUuid& uuid) override;
    void updateBook(const QString& authToken,
                    const domain::entities::Book& book) override;
    void changeBookCover(const QString& authToken, const QUuid& uuid,
                         const QString& path) override;
    void deleteBookCover(const QString& authToken, const QUuid& uuid) override;
    void getBooksMetaData(const QString& authToken) override;
    void getCoverForBook(const QString& authToken, const QUuid& uuid) override;
    void downloadBookMedia(const QString& authToken,
                           const QUuid& uuid) override;

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
    QJsonArray renameProperties(const QJsonArray& tags,
                                TagNamingStyle namingStyle);
    void renameJsonObjectKey(QJsonObject& jsonObject, const QString& oldKeyName,
                             const QString& newKeyName);

    ILibraryStorageAccess* m_bookStorageAccess;
};

}  // namespace adapters::gateways
