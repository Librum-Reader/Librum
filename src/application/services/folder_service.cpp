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
                syncWithServer();
            });
}

void FolderService::syncWithServer()
{
    m_folderStorageGateway->fetchFolders(m_authenticationToken);
}

Folder* FolderService::getRootFolder()
{
    return m_rootFolder.get();
}

Folder* FolderService::getFolder(const QUuid& uuid)
{
    return getFolderHelper(uuid, m_rootFolder.get());
}

Folder* FolderService::getFolderHelper(const QUuid& uuid, Folder* curr)
{
    if(curr->getUuid() == uuid)
        return curr;

    for(auto& child : curr->getChildren())
    {
        auto result = getFolderHelper(uuid, child.get());
        if(result != nullptr)
            return result;
    }

    return nullptr;
}

bool FolderService::createFolder(const QString& name, QString color,
                                 QString icon, QString description,
                                 const QUuid& parent)
{
    Folder* parentFolder = nullptr;
    if(parent.isNull())  // Set the root folder as the parent
        parentFolder = m_rootFolder.get();
    else
        parentFolder = getFolder(parent);

    if(parentFolder == nullptr)
    {
        qWarning() << "Creating folder failed. Could not find parent folder "
                      "with uuid: "
                   << parent;
        return false;
    }

    auto newFolder = std::make_unique<Folder>(name, color, icon, description);
    createFolderHelper(std::move(newFolder), parentFolder);

    saveChanges();
    return true;
}

QList<QString> FolderService::deleteFolder(const QUuid& uuid)
{
    auto folder = getFolder(uuid);
    if(folder == nullptr)
    {
        qWarning()
            << "Deleting folder failed. Could not find folder with uuid: "
            << uuid;
        return {};
    }

    // We return a list of all the folders that are deleted (all
    // descendents of the folder are deleted as well).
    auto listOfDescendents = getUuidsOfAllDescendents(*folder);
    listOfDescendents.push_front(uuid.toString(QUuid::WithoutBraces));

    deleteFolderHelper(folder);

    saveChanges();
    return listOfDescendents;
}

void FolderService::updateFolder(const domain::entities::Folder& folder)
{
    auto realFolder = getFolder(folder.getUuid());
    if(realFolder == nullptr)
    {
        qWarning()
            << "Updating folder failed. Could not find folder with uuid: "
            << folder.getUuid();
        return;
    }

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
    if(currFolder == nullptr || destFolder == nullptr)
    {
        qWarning() << "Folder move operation failed. Could not find source or "
                      "destination";
        return false;
    }

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
    // the original folder's memory.
    auto currFolderCopy = std::make_unique<Folder>(*currFolder);

    deleteFolderHelper(currFolder);
    createFolderHelper(std::move(currFolderCopy), destFolder);

    saveChanges();
    return true;
}

void FolderService::setupUserData(const QString& token, const QString& email)
{
    m_authenticationToken = token;
    m_localLibraryTracker->setLibraryOwner(email);
    m_rootFolder->getChildren().clear();

    auto folder = m_localLibraryTracker->loadFolders();
    if(folder.getName() == "invalid")  // No local library exists yet
        createDefaultFolder();
    else
        overwriteRootFolderWith(folder);

    // Trigger a timeout manually at the start for the initial folder loading
    m_fetchChangesTimer.start();
    QMetaObject::invokeMethod(&m_fetchChangesTimer, "timeout");
}

void FolderService::createDefaultFolder()
{
    auto defaultFolder = std::make_unique<Folder>(
        "Archive", "default", "folder", "A folder for archived books");
    defaultFolder->setIndexInParent(0);

    m_rootFolder->addChild(std::move(defaultFolder));
}

void FolderService::overwriteRootFolderWith(Folder& folder)
{
    m_rootFolder->setUuid(folder.getUuid());
    m_rootFolder->setLastModified(folder.getLastModified());
    m_rootFolder->getChildren().clear();

    // We need to add the folder's children to the current root element,
    // since a pointer of the root element was already passed to the folders
    // model, so we can't simply overwrite it, else we'd invalidate the pointer.
    for(auto& child : folder.getChildren())
        m_rootFolder->addChild(std::move(child));

    // JSON format does not guarantee that the children are in the correct order
    // when loaded, so we need to sort them.
    m_rootFolder->sortDescendents();
}

void FolderService::clearUserData()
{
    m_localLibraryTracker->clearLibraryOwner();

    m_rootFolder->getChildren().clear();
    m_rootFolder->setUuid(QUuid());
    m_rootFolder->setLastModified(QDateTime::currentDateTime());

    m_fetchChangesTimer.stop();
}

void FolderService::saveChanges()
{
    m_localLibraryTracker->saveFolders(*m_rootFolder);
    m_folderStorageGateway->updateFolder(m_authenticationToken, *m_rootFolder);
}

void FolderService::processFetchedFolders(Folder& remoteRoot)
{
    // This occurs when there is no folder on the server yet. If that's the
    // case, we just want to sync whatever we have locally to the server.
    if(remoteRoot.getName() == "")
    {
        m_folderStorageGateway->updateFolder(m_authenticationToken,
                                             *m_rootFolder);
        return;
    }

    // This occurs when there is folder data on the server, but it wasn never
    // synced with the client (thus the root uuids are different). In this case,
    // we just want to overwrite the local folders with the remote folders.
    auto localRoot = getFolder(remoteRoot.getUuid());
    if(localRoot == nullptr)
    {
        emit beginModelReset();
        overwriteRootFolderWith(remoteRoot);
        emit endModelReset();
    }
    else
    {
        emit beginModelReset();
        updateFoldersRecursively(localRoot, remoteRoot);
        m_rootFolder->sortDescendents();
        emit endModelReset();
    }

    saveChanges();
}

void FolderService::updateFoldersRecursively(Folder* curr, Folder& remoteRoot)
{
    Folder* remoteFolder = nullptr;
    if(curr == m_rootFolder.get())
        remoteFolder = &remoteRoot;
    else
        remoteFolder = remoteRoot.getDescendant(curr->getUuid());

    // This occurs when the remote folder was added locally, and thus moved
    // from. We just skip it since it was already dealt with.
    if(remoteFolder == nullptr)
        return;

    auto currLastModified = curr->getLastModified().toSecsSinceEpoch();
    auto remoteLastModified =
        remoteFolder->getLastModified().toSecsSinceEpoch();
    if(remoteLastModified > currLastModified)
    {
        curr->updateProperties(*remoteFolder);

        // We don't want to emit refreshFolder for the root folder
        if(curr->getName() != "ROOT")
            emit refreshFolder(curr->getParent(), curr->getIndexInParent());

        // If the remote folder has changed, we treat it as the source of
        // truth and replace the whole subtree of the current folder with the
        // remote subtree.
        curr->getChildren().clear();
        for(auto& child : remoteFolder->getChildren())
        {
            child->setIndexInParent(curr->childCount());
            curr->addChild(std::make_unique<Folder>(*child));
        }

        return;
    }

    for(auto& child : curr->getChildren())
        updateFoldersRecursively(child.get(), remoteRoot);
}

QList<QString> FolderService::getUuidsOfAllDescendents(const Folder& folder)
{
    QList<QString> uuids;
    for(auto& child : folder.getChildren())
    {
        uuids.push_back(child->getUuid().toString(QUuid::WithoutBraces));
        auto childUuids = getUuidsOfAllDescendents(*child);
        uuids.append(childUuids);
    }

    return uuids;
}

void FolderService::createFolderHelper(std::unique_ptr<Folder> folder,
                                       Folder* parent)
{
    folder->setIndexInParent(parent->childCount());

    emit beginInsertFolder(parent, parent->childCount());
    parent->addChild(std::move(folder));
    emit endInsertFolder();

    parent->updateLastModified();
}

void FolderService::deleteFolderHelper(Folder* folder)
{
    int indexInParent = folder->getIndexInParent();
    auto parent = folder->getParent();

    emit beginRemoveFolder(folder->getParent(), indexInParent);
    auto success = parent->removeChild(folder->getUuid());
    emit endRemoveFolder();

    if(success)
    {
        parent->decreaseChildIndiciesIfBiggerThan(indexInParent);
        parent->updateLastModified();
    }
}

}  // namespace application::services