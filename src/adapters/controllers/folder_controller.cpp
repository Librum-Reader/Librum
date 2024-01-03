#include "folder_controller.hpp"

namespace adapters::controllers
{

FolderController::FolderController(application::IFolderService* folderService) :
    m_folderService(folderService)
{
}

}  // namespace adapters::controllers