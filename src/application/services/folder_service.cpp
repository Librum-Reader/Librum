#include "folder_service.hpp"
#include <QDebug>

namespace application::services
{

using domain::entities::Folder;

FolderService::FolderService(IFolderStorageGateway* folderStorageGateway,
                             ILocalLibraryTracker* localLibraryTracker) :
    m_folderStorageGateway(folderStorageGateway),
    m_localLibraryTracker(localLibraryTracker),
    m_rootFolder(std::make_unique<Folder>("ROOT", "", "", "", QUuid()))
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

bool FolderService::createFolder(const QString& name, QString color,
                                 QString icon, QString description,
                                 const QUuid& parent)
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
    parentFolder->addChild(
        std::make_unique<Folder>(name, color, icon, description));
    emit endInsertFolder();

    saveChanges();
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

    saveChanges();
    return true;
}

void FolderService::updateFolder(const domain::entities::Folder& folder)
{
    auto realFolder = getFolder(folder.getUuid());
    realFolder->setName(folder.getName());
    realFolder->setColor(folder.getColor());
    realFolder->setIcon(folder.getIcon());
    realFolder->setDescription(folder.getDescription());

    saveChanges();
    emit refreshFolder(realFolder->getParent(), realFolder->getIndexInParent());
}

bool FolderService::moveFolder(const QUuid& uuid, const QUuid& destUuid)
{
    auto currFolder = getFolder(uuid);
    auto destFolder = getFolder(destUuid);

    // We don't need to do anything if dest is already the parent
    if(currFolder->getParent()->getUuid() == destFolder->getUuid())
        return false;

    if(destFolder->isChildOf(*currFolder))
    {
        qWarning() << "Folder move operation failed. Attempted to move folder "
                      "into child";
        return false;
    }

    // Make copy since we delete the currFolder from its parent which frees
    // it's memory.
    auto currFolderCopy = std::make_unique<Folder>(*currFolder);

    emit beginRemoveFolder(currFolder->getParent(),
                           currFolder->getIndexInParent());
    currFolder->getParent()->removeChild(uuid);
    emit endRemoveFolder();

    emit beginInsertFolder(destFolder, destFolder->childCount());
    destFolder->addChild(std::move(currFolderCopy));
    emit endInsertFolder();

    saveChanges();
    return true;
}

void FolderService::setupUserData(const QString& token, const QString& email)
{
    Q_UNUSED(token)
    m_localLibraryTracker->setLibraryOwner(email);

    auto folder = m_localLibraryTracker->loadFolders();

    // This occurs when the user has no library yet. If this occurs, we want to
    // create a default folder under the root folder for them.
    if(folder.getName() == "invalid")
    {
        m_rootFolder->addChild(std::make_unique<Folder>(
            "Archive", "default", "folder", "Unused books"));
        return;
    }

    // We need to add the folder's children to the current root element, since a
    // pointer of the root element was already passed to the model, so we can't
    // simply overwrite it, else we'd invalidate the pointer.
    for(auto& child : folder.getChildren())
        m_rootFolder->addChild(std::move(child));
}

void FolderService::clearUserData()
{
    m_localLibraryTracker->clearLibraryOwner();
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

void FolderService::saveChanges()
{
    m_localLibraryTracker->saveFolders(*m_rootFolder);
}

}  // namespace application::services