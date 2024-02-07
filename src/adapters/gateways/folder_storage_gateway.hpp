#pragma once
#include <QObject>
#include "adapters_export.hpp"
#include "i_folder_storage_access.hpp"
#include "i_folder_storage_gateway.hpp"

namespace adapters::gateways
{

class ADAPTERS_EXPORT FolderStorageGateway
    : public application::IFolderStorageGateway
{
    Q_OBJECT

public:
    FolderStorageGateway(IFolderStorageAccess* folderStorageAccess);

    void updateFolder(const QString& authToken,
                      const domain::entities::Folder& folder) override;
    void fetchFolders(const QString& authToken) override;

private slots:
    void processFetchedFolders(QByteArray jsonFolders);

private:
    void assignParentsToFolders(domain::entities::Folder& folder);

    IFolderStorageAccess* m_folderStorageAccess;
};

}  // namespace adapters::gateways