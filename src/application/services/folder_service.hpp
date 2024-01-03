#pragma once
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

private:
    IFolderStorageGateway* m_folderStorageGateway;
};

}  // namespace application::services