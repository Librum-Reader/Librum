#include "folder_service.hpp"

namespace application::services
{

FolderService::FolderService(IFolderStorageGateway* folderStorageGateway) :
    m_folderStorageGateway(folderStorageGateway)
{
}

}  // namespace application::services