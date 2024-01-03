#include "folder_storage_gateway.hpp"

namespace adapters::gateways
{

FolderStorageGateway::FolderStorageGateway(
    IFolderStorageAccess* folderStorageAccess) :
    m_folderStorageAccess(folderStorageAccess)
{
}

}  // namespace adapters::gateways