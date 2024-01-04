#include "folder_service.hpp"

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

bool FolderService::createFolder(QString name, QUuid parent)
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

bool FolderService::deleteFolder(QUuid uuid)
{
}

void FolderService::renameFolder(QUuid uuid, QString newName)
{
    auto folder = getFolder(uuid);
    folder->setName(newName);

    emit refreshFolder(folder->getParent(), folder->getIndexInParent());
}

Folder* FolderService::getFolder(const QUuid& uuid)
{
    return getFolderHelper(uuid, m_rootFolder.get());
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