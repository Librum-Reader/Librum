#include "folder_service.hpp"

namespace application::services
{

using domain::entities::Folder;

FolderService::FolderService(IFolderStorageGateway* folderStorageGateway) :
    m_folderStorageGateway(folderStorageGateway),
    m_rootFolder(std::make_unique<Folder>("ROOT", QUuid()))
{
    auto firstLvl1 = std::make_unique<Folder>("First Lvl 1");

    auto firstLvl2 = std::make_unique<Folder>("First Lvl 2");
    firstLvl1->addChild(std::move(firstLvl2));

    auto secondLvl2 = std::make_unique<Folder>("Second Lvl 2");
    firstLvl1->addChild(std::move(secondLvl2));

    auto thirdLvl2 = std::make_unique<Folder>("Third Lvl 2");
    firstLvl1->addChild(std::move(thirdLvl2));

    m_rootFolder->addChild(std::move(firstLvl1));

    auto secondLvl1 = std::make_unique<Folder>("Second Lvl 1");

    auto fourthLvl2 = std::make_unique<Folder>("Fourth Lvl 2");
    secondLvl1->addChild(std::move(fourthLvl2));

    m_rootFolder->addChild(std::move(secondLvl1));
}

Folder* FolderService::getRootFolder()
{
    return m_rootFolder.get();
}

bool FolderService::createFolder(QString name, QUuid parent)
{
    if(name.isEmpty())
        return false;

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