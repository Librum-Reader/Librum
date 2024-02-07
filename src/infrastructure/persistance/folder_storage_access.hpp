#pragma once
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QObject>
#include "i_folder_storage_access.hpp"

namespace infrastructure::persistence
{

class FolderStorageAccess : public adapters::IFolderStorageAccess
{
    Q_OBJECT

public:
    FolderStorageAccess();

    void updateFolder(const QString& authToken,
                      const QByteArray& jsonFolder) override;
    void fetchFolders(const QString& authToken) override;

private:
    QNetworkRequest createRequest(QUrl url, const QString& authToken);

    QNetworkAccessManager m_networkAccessManager;
    QString domain;
};

}  // namespace infrastructure::persistence