#include "folder_controller.hpp"

namespace adapters::controllers
{

using domain::entities::Folder;

FolderController::FolderController(application::IFolderService* folderService) :
    m_folderService(folderService),
    m_foldersModel(m_folderService->getRootFolder())
{
    m_foldersProxyModel.setSourceModel(&m_foldersModel);
    m_iconProxyModel.setSourceModel(&m_iconModel);

    // Insertion signals
    connect(m_folderService, &application::IFolderService::beginInsertFolder,
            &m_foldersModel, &data_models::FoldersModel::beginInsertFolder);

    connect(m_folderService, &application::IFolderService::endInsertFolder,
            &m_foldersModel, &data_models::FoldersModel::endInsertFolder);

    // Removal signals
    connect(m_folderService, &application::IFolderService::beginRemoveFolder,
            &m_foldersModel, &data_models::FoldersModel::beginRemoveFolder);

    connect(m_folderService, &application::IFolderService::endRemoveFolder,
            &m_foldersModel, &data_models::FoldersModel::endRemoveFolder);

    // Refresh signals
    connect(m_folderService, &application::IFolderService::refreshFolder,
            &m_foldersModel, &data_models::FoldersModel::refreshFolder);

    // Reset signals
    connect(m_folderService, &application::IFolderService::beginModelReset,
            &m_foldersModel, &data_models::FoldersModel::beginModelReset);

    connect(m_folderService, &application::IFolderService::endModelReset,
            &m_foldersModel, &data_models::FoldersModel::endModelReset);
}

data_models::FoldersProxyModel* FolderController::getFoldersModel()
{
    return &m_foldersProxyModel;
}

data_models::IconProxyModel* FolderController::getIconModel()
{
    return &m_iconProxyModel;
}

void FolderController::syncWithServer()
{
    m_folderService->syncWithServer();
}

dtos::FolderDto FolderController::getFolder(QString uuid)
{
    auto folder = m_folderService->getFolder(QUuid(uuid));

    return dtos::FolderDto {
        .uuid = folder->getUuid().toString(QUuid::WithoutBraces),
        .name = folder->getName(),
        .color = folder->getColor(),
        .icon = folder->getIcon(),
        .description = folder->getDescription(),
    };
}

bool FolderController::createFolder(QString name, QString color, QString icon,
                                    QString description, QString parent)
{
    auto success = m_folderService->createFolder(name, color, icon, description,
                                                 QUuid(parent));
    if(success && !parent.isEmpty())
        emit expandFolder(parent);

    return success;
}

QVariantList FolderController::deleteFolder(QString uuid)
{
    auto uuids = m_folderService->deleteFolder(QUuid(uuid));

    QVariantList list;
    for(auto& id : uuids)
        list.append(QVariant::fromValue(id));

    return list;
}

void FolderController::updateFolder(QString uuid, QString name, QString color,
                                    QString icon, QString description)
{
    Folder folder(name, color, icon, description, QUuid(uuid));
    m_folderService->updateFolder(folder);
}

bool FolderController::moveFolder(QString uuid, QString destUuid)
{
    bool destinationIsRoot = false;
    if(destUuid == "")
    {
        destUuid = m_folderService->getRootFolder()->getUuid().toString();
        destinationIsRoot = true;
    }

    auto success = m_folderService->moveFolder(QUuid(uuid), QUuid(destUuid));
    if(success && !destinationIsRoot)
        emit expandFolder(destUuid);

    return success;
}

}  // namespace adapters::controllers