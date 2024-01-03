#include "folder_service.hpp"

namespace application::services
{

using domain::entities::Folder;

FolderService::FolderService(IFolderStorageGateway* folderStorageGateway) :
    m_folderStorageGateway(folderStorageGateway),
    m_rootFolder(std::make_unique<Folder>("ROOT", 0, QUuid()))
{
    auto firstLvl1 =
        std::make_unique<Folder>("First Lvl 1", m_rootFolder->childCount());

    auto firstLvl2 =
        std::make_unique<Folder>("First Lvl 2", firstLvl1->childCount());
    firstLvl1->addChild(std::move(firstLvl2));

    auto secondLvl2 =
        std::make_unique<Folder>("Second Lvl 2", firstLvl1->childCount());
    firstLvl1->addChild(std::move(secondLvl2));

    auto thirdLvl2 =
        std::make_unique<Folder>("Third Lvl 2", firstLvl1->childCount());
    firstLvl1->addChild(std::move(thirdLvl2));

    m_rootFolder->addChild(std::move(firstLvl1));

    auto secondLvl1 =
        std::make_unique<Folder>("Second Lvl 1", m_rootFolder->childCount());

    auto fourthLvl2 =
        std::make_unique<Folder>("Fourth Lvl 2", secondLvl1->childCount());
    secondLvl1->addChild(std::move(fourthLvl2));

    m_rootFolder->addChild(std::move(secondLvl1));

    ;
}

Folder* FolderService::getRootFolder()
{
    return m_rootFolder.get();
}

}  // namespace application::services