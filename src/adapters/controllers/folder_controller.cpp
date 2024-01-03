#include "folder_controller.hpp"

namespace adapters::controllers
{

FolderController::FolderController(application::IFolderService* folderService) :
    m_folderService(folderService),
    m_foldersModel(m_folderService->getRootFolder())
{
    m_foldersProxyModel.setSourceModel(&m_foldersModel);
}

data_models::FoldersProxyModel* FolderController::getFoldersModel()
{
    return &m_foldersProxyModel;
}

}  // namespace adapters::controllers