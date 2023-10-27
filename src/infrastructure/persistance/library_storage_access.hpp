#pragma once
#include <QHttpMultiPart>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QString>
#include "i_library_storage_access.hpp"

namespace infrastructure::persistence
{

class LibraryStorageAccess : public adapters::ILibraryStorageAccess
{
    Q_OBJECT

public:
    LibraryStorageAccess();

    void createBook(const QString& authToken,
                    const QJsonObject& jsonBook) override;
    void deleteBook(const QString& authToken, const QUuid& uuid) override;
    void updateBook(const QString& authToken,
                    const QJsonObject& jsonBook) override;
    void uploadBookCover(const QString& authToken, const QUuid& uuid,
                         const QString& path) override;
    void deleteBookCover(const QString& authToken, const QUuid& uuid) override;
    void getBooksMetaData(const QString& authToken) override;
    void downloadCoverForBook(const QString& authToken,
                              const QUuid& uuid) override;
    void downloadBookMedia(const QString& authToken,
                           const QUuid& uuid) override;

private slots:
    void processGettingBooksMetaDataResult(QNetworkReply* reply);

private:
    QNetworkRequest createRequest(const QUrl& url, const QString& authToken);
    void uploadBookMedia(const QString& uuid, const QString& filePath,
                         const QString& authToken);
    bool addFilePartToMultiPart(QHttpMultiPart* bookData, const QString& path);


    QNetworkAccessManager m_networkAccessManager;
    QString domain;
};

}  // namespace infrastructure::persistence
