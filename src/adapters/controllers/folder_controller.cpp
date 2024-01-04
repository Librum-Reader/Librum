#include "folder_controller.hpp"

namespace adapters::controllers
{

using domain::entities::Folder;

FolderController::FolderController(application::IFolderService* folderService) :
    m_folderService(folderService),
    m_foldersModel(m_folderService->getRootFolder())
{
    m_foldersProxyModel.setSourceModel(&m_foldersModel);

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
}

data_models::FoldersProxyModel* FolderController::getFoldersModel()
{
    return &m_foldersProxyModel;
}

dtos::FolderDto FolderController::getFolder(QString uuid)
{
    auto folder = m_folderService->getFolder(QUuid(uuid));

    return dtos::FolderDto {
        .uuid = folder->getUuid().toString(QUuid::WithoutBraces),
        .name = folder->getName(),
    };
}

bool FolderController::createFolder(QString name, QString parent)
{
    return m_folderService->createFolder(name, QUuid(parent));
}

bool FolderController::deleteFolder(QString uuid)
{
    return m_folderService->deleteFolder(QUuid(uuid));
}

void FolderController::updateFolder(QString uuid, QString name, QString icon,
                                    QString description)
{
    Folder folder(name, QUuid(uuid));
    m_folderService->updateFolder(folder);
}

}  // namespace adapters::controllers