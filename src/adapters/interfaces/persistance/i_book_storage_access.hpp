#pragma once
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <vector>

namespace adapters
{

/**
 * The IAuthenticationAccess class makes the API calls to the authentication
 * server.
 */
class IBookStorageAccess : public QObject
{
    Q_OBJECT

public:
    virtual ~IBookStorageAccess() noexcept = default;

    virtual void createBook(const QString& authToken,
                            const QJsonObject& jsonBook) = 0;
    virtual void deleteBook(const QString& authToken, const QUuid& uuid) = 0;
    virtual void updateBook(const QString& authToken,
                            const QJsonObject& jsonBook) = 0;
    virtual void getBooksMetaData(const QString& authToken) = 0;
    virtual void downloadBook(const QString& authToken, const QUuid& uuid) = 0;

signals:
    void creatingBookFinished(bool success, const QString& reason);
    void deletingBookFinished(bool success, const QString& reason);
    void updatingBookFinished(bool success, const QString& reason);
    void gettingBooksMetaDataFinished(std::vector<QJsonObject>& metaData);
    void downloadingBookFinisdhed(const QUuid& uuid, const QByteArray& data);
};

}  // namespace adapters