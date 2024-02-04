#include "folder_storage_gateway.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace adapters::gateways
{

using domain::entities::Folder;

FolderStorageGateway::FolderStorageGateway(
    IFolderStorageAccess* folderStorageAccess) :
    m_folderStorageAccess(folderStorageAccess)
{
    connect(m_folderStorageAccess, &IFolderStorageAccess::foldersFetched, this,
            &FolderStorageGateway::processFetchedFolders);
}

void FolderStorageGateway::updateFolder(const QString& authToken,
                                        const Folder& folder)
{
    auto jsonFolder = folder.toJson();

    // The server expects uuid to be called guid, so replace all occurences
    jsonFolder.replace("uuid", "guid");

    m_folderStorageAccess->updateFolder(authToken, jsonFolder);
}

void FolderStorageGateway::fetchFolders(const QString& authToken)
{
    m_folderStorageAccess->fetchFolders(authToken);
}

void FolderStorageGateway::processFetchedFolders(const QByteArray& jsonFolder)
{
    if(jsonFolder.isEmpty())
    {
        Folder empty("", "", "", "", QUuid());
        emit foldersFetched(empty);
        return;
    }

    auto folderObj = QJsonDocument::fromJson(jsonFolder).object();
    auto folder = Folder::fromJson(folderObj, nullptr);

    // The folders on the backend do not have a parent property, they are only
    // kept as a list. So we need to assign the parent to each folder.
    assignParentsToFolders(folder);

    emit foldersFetched(folder);
}

void FolderStorageGateway::assignParentsToFolders(Folder& folder)
{
    for(auto& child : folder.getChildren())
    {
        child->setParent(&folder);
        assignParentsToFolders(*child);
    }
}

}  // namespace adapters::gateways