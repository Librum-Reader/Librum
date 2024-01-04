#pragma once
#include <memory>
#include "application_export.hpp"
#include "i_folder_service.hpp"
#include "i_folder_storage_gateway.hpp"

namespace application::services
{

class APPLICATION_EXPORT FolderService : public IFolderService
{
    Q_OBJECT

public:
    FolderService(IFolderStorageGateway* folderStorageGateway);

    domain::entities::Folder* getRootFolder() override;
    domain::entities::Folder* getFolder(const QUuid& uuid) override;
    bool createFolder(const QString& name, const QUuid& parent) override;
    bool deleteFolder(const QUuid& uuid) override;
    void updateFolder(const domain::entities::Folder& folder) override;

private:
    domain::entities::Folder* getFolderHelper(const QUuid& uuid,
                                              domain::entities::Folder* parent);

    IFolderStorageGateway* m_folderStorageGateway;
    // This is the root of all folders and will NOT be displayed.
    std::unique_ptr<domain::entities::Folder> m_rootFolder;
};

}  // namespace application::services