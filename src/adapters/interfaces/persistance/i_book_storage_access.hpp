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
    virtual void uploadBookCover(const QString& authToken, const QUuid& uuid,
                                 const QString& path) = 0;
    virtual void deleteBookCover(const QString& authToken,
                                 const QUuid& uuid) = 0;
    virtual void downloadCoverForBook(const QString& authToken,
                                      const QUuid& uuid) = 0;
    virtual void getBooksMetaData(const QString& authToken) = 0;
    virtual void downloadBookMedia(const QString& authToken,
                                   const QUuid& uuid) = 0;

signals:
    void deletingBookFinished(bool success, const QString& reason);
    void updatingBookFinished(bool success, const QString& reason);
    void downloadingBookMediaChunkReady(const QByteArray& data,
                                        const bool isChunkLast,
                                        const QUuid& uuid,
                                        const QString& format);
    void downloadingBookMediaFinished(const QByteArray& data,
                                      const bool isChunkLast,
                                      const QUuid& uuid,
                                      const QString& format);
    void gettingBooksMetaDataFinished(std::vector<QJsonObject>& metaData);
    void downloadingBookCoverFinished(const QByteArray& data,
                                      const QUuid& uuid);
    void uploadLimitReached();
    void storageLimitExceeded();
};

}  // namespace adapters
