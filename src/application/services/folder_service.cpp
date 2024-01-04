#include "folder_service.hpp"
#include <ranges>

namespace application::services
{

using domain::entities::Folder;

FolderService::FolderService(IFolderStorageGateway* folderStorageGateway) :
    m_folderStorageGateway(folderStorageGateway),
    m_rootFolder(std::make_unique<Folder>("ROOT", QUuid()))
{
}

Folder* FolderService::getRootFolder()
{
    return m_rootFolder.get();
}

Folder* FolderService::getFolder(const QUuid& uuid)
{
    return getFolderHelper(uuid, m_rootFolder.get());
}

bool FolderService::createFolder(const QString& name, const QUuid& parent)
{
    Folder* parentFolder = nullptr;
    // This means that the folder should be created in the root folder.
    if(parent.isNull())
        parentFolder = m_rootFolder.get();
    else
        parentFolder = getFolder(parent);

    if(parentFolder == nullptr)
        return false;

    emit beginInsertFolder(parentFolder, parentFolder->childCount());
    parentFolder->addChild(std::make_unique<Folder>(name));
    emit endInsertFolder();

    return true;
}

bool FolderService::deleteFolder(const QUuid& uuid)
{
    auto folder = getFolder(uuid);
    if(folder == nullptr)
        return false;

    auto parent = folder->getParent();

    emit beginRemoveFolder(parent, folder->getIndexInParent());
    parent->removeChild(uuid);
    emit endRemoveFolder();

    return true;
}

void FolderService::updateFolder(const domain::entities::Folder& folder)
{
    auto realFolder = getFolder(folder.getUuid());
    realFolder->setName(folder.getName());

    emit refreshFolder(realFolder->getParent(), realFolder->getIndexInParent());
}

Folder* FolderService::getFolderHelper(const QUuid& uuid,
                                       domain::entities::Folder* parent)
{
    if(parent->getUuid() == uuid)
        return parent;

    for(auto& child : parent->getChildren())
    {
        auto result = getFolderHelper(uuid, child.get());
        if(result != nullptr)
            return result;
    }

    return nullptr;
}

}  // namespace application::services