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

private:
    QNetworkRequest createRequest(QUrl url);

    QNetworkAccessManager m_networkAccessManager;
};

}  // namespace infrastructure::persistence