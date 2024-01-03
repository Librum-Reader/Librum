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

private:
    IFolderStorageAccess* m_folderStorageAccess;
};

}  // namespace adapters::gateways