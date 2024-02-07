#include "folder_service.hpp"
#include <QDebug>

namespace application::services
{

using domain::entities::Folder;

FolderService::FolderService(IFolderStorageGateway* folderStorageGateway,
                             ILocalLibraryTracker* localLibraryTracker) :
    m_folderStorageGateway(folderStorageGateway),
    m_localLibraryTracker(localLibraryTracker),
    m_rootFolder(std::make_unique<Folder>("ROOT", "", "", ""))
{
    connect(m_folderStorageGateway, &IFolderStorageGateway::foldersFetched,
            this, &FolderService::processFetchedFolders);

    // Fetch changes timer
    m_fetchChangesTimer.setInterval(m_fetchChangesInterval);
    connect(&m_fetchChangesTimer, &QTimer::timeout, this,
            [this]()
            {
                m_folderStorageGateway->fetchFolders(m_authenticationToken);
            });
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

    // Set the index in parent to the current child count, since we're adding
    // the folder to the end of the parent's children.
    auto child = std::make_unique<Folder>(name, color, icon, description);
    child->setIndexInParent(parentFolder->childCount());

    emit beginInsertFolder(parentFolder, parentFolder->childCount());
    parentFolder->addChild(std::move(child));
    emit endInsertFolder();

    parentFolder->updateLastModified();
    saveChanges();
    return true;
}

bool FolderService::deleteFolder(const QUuid& uuid)
{
    auto folder = getFolder(uuid);
    if(folder == nullptr)
        return false;

    auto parent = folder->getParent();
    auto indexInParent = folder->getIndexInParent();

    emit beginRemoveFolder(parent, indexInParent);
    parent->removeChild(uuid);
    emit endRemoveFolder();

    parent->decreaseChildIndiciesIfBiggerThan(indexInParent);
    parent->updateLastModified();
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

    realFolder->updateLastModified();
    saveChanges();
    emit refreshFolder(realFolder->getParent(), realFolder->getIndexInParent());
}

bool FolderService::moveFolder(const QUuid& uuid, const QUuid& destUuid)
{
    auto currFolder = getFolder(uuid);
    auto destFolder = getFolder(destUuid);

    // We don't need to do anything if dest already is the parent.
    if(currFolder->getParent()->getUuid() == destFolder->getUuid())
        return false;

    if(destFolder->isDescendentOf(*currFolder, m_rootFolder->getUuid()))
    {
        qWarning() << "Folder move operation failed. Attempted to move folder "
                      "into child";
        return false;
    }

    // Make copy since we delete the currFolder from its parent which frees
    // it's memory.
    auto currFolderCopy = std::make_unique<Folder>(*currFolder);

    auto currentParent = currFolder->getParent();
    // Since we remove the child from it's old parent, all other children's
    // indecies need to be adjusted.
    currentParent->decreaseChildIndiciesIfBiggerThan(
        currFolderCopy->getIndexInParent());

    emit beginRemoveFolder(currFolder->getParent(),
                           currFolder->getIndexInParent());
    currentParent->removeChild(uuid);
    currentParent->updateLastModified();
    emit endRemoveFolder();

    currFolderCopy->setIndexInParent(destFolder->childCount());
    emit beginInsertFolder(destFolder, destFolder->childCount());
    destFolder->addChild(std::move(currFolderCopy));
    emit endInsertFolder();

    destFolder->updateLastModified();
    saveChanges();
    return true;
}

void FolderService::setupUserData(const QString& token, const QString& email)
{
    m_authenticationToken = token;
    m_localLibraryTracker->setLibraryOwner(email);

    auto folder = m_localLibraryTracker->loadFolders();

    // This occurs when the user has no library yet. If this occurs, we want to
    // create a default folder under the root folder for them.
    if(folder.getName() == "invalid")
    {
        auto defaultFolder = std::make_unique<Folder>(
            "Archive", "default", "folder", "A folder for archived books");
        defaultFolder->setIndexInParent(0);

        m_rootFolder->addChild(std::move(defaultFolder));
    }
    else
    {
        // Update the root folder's uuid with the stored uuid
        m_rootFolder->setUuid(folder.getUuid());
        m_rootFolder->setLastModified(folder.getLastModified());

        // We need to add the folder's children to the current root element,
        // since a pointer of the root element was already passed to the model,
        // so we can't simply overwrite it, else we'd invalidate the pointer.
        for(auto& child : folder.getChildren())
            m_rootFolder->addChild(std::move(child));

        // JSON format does not guarantee that the children are sorted.
        m_rootFolder->sortDescendents();
    }

    // Trigger a timeout manually at the start for the initial folder loading
    m_fetchChangesTimer.start();
    QMetaObject::invokeMethod(&m_fetchChangesTimer, "timeout");
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
    m_folderStorageGateway->updateFolder(m_authenticationToken, *m_rootFolder);
}

void FolderService::processFetchedFolders(Folder& remoteRoot)
{
    // This occurs when there is no folder on the server yet. We want to create
    // a default folder for the user.
    if(remoteRoot.getName().isEmpty())
    {
        m_folderStorageGateway->updateFolder(m_authenticationToken,
                                             *m_rootFolder);
        return;
    }

    // This occurs when there is folder data on the server, but there is no
    // local folder data on the client yet. E.g. when starting the application
    // for the first time from a new machine, while folders already exist on the
    // server.
    // In this case we want to just take all of the data from the server by
    // setting the rootFolder's uuid, removing the child created by default and
    // assigning the server folder's children to the root folder.
    auto localRoot = getFolder(remoteRoot.getUuid());
    if(localRoot == nullptr)
    {
        m_rootFolder->setUuid(remoteRoot.getUuid());
        m_rootFolder->removeChild(m_rootFolder->getChildAtIndex(0)->getUuid());

        emit beginModelReset();
        for(auto& child : remoteRoot.getChildren())
            m_rootFolder->addChild(std::move(child));

        m_rootFolder->sortDescendents();
        emit endModelReset();
    }
    // We don't need update the local folders if we copy over everything from
    // the server since local and server will be exactly the same.
    else
    {
        updateFoldersRecursively(localRoot, remoteRoot);
        m_rootFolder->sortDescendents();
    }

    saveChanges();
}

void FolderService::updateFoldersRecursively(Folder* current,
                                             Folder& remoteRoot)
{
    Folder* remoteFolder = nullptr;
    if(current == m_rootFolder.get())
        remoteFolder = &remoteRoot;
    else
        remoteFolder = remoteRoot.getDescendant(current->getUuid());

    // This occurs when the remote folder was added, and thus moved from, during
    // the 'addMissingChildrenToFolder' step. We just skip it.
    if(remoteFolder == nullptr)
        return;

    auto currLastModified = current->getLastModified().toSecsSinceEpoch();
    auto remoteLastModified =
        remoteFolder->getLastModified().toSecsSinceEpoch();
    if(remoteLastModified > currLastModified)
    {
        current->updateProperties(*remoteFolder);

        // We don't want to emit refreshFolder for the root folder
        if(current->getName() != "ROOT")
        {
            emit refreshFolder(current->getParent(),
                               current->getIndexInParent());
        }

        // If the remote folder has changed, we treat it as the source of
        // truth and replace the whole subtree of the current folder with the
        // remote subtree.
        QList<QUuid> foldersToRemove;
        for(auto& child : current->getChildren())
            foldersToRemove.push_back(child->getUuid());

        for(auto& uuid : foldersToRemove)
        {
            emit beginRemoveFolder(current, current->getIndexOfChild(uuid));
            current->removeChild(uuid);
            emit endRemoveFolder();
        }

        for(auto& child : remoteFolder->getChildren())
        {
            child->setParent(current);
            emit beginInsertFolder(current, current->childCount());
            current->addChild(std::move(child));
            emit endInsertFolder();
        }

        return;
    }

    for(auto& child : current->getChildren())
    {
        updateFoldersRecursively(child.get(), remoteRoot);
    }
}

}  // namespace application::services