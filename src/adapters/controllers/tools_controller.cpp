#include "tools_controller.hpp"

namespace adapters::controllers
{

ToolsController::ToolsController(application::IToolsService* toolsService) :
    m_toolsService(toolsService)
{
}

void ToolsController::mergePdfs(const QList<QString>& filePaths)
{
    m_toolsService->mergePdfs(filePaths);
}

}  // namespace adapters::controllers