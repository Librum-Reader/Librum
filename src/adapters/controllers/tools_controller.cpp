#include "tools_controller.hpp"

namespace adapters::controllers
{

ToolsController::ToolsController(application::IToolsService* toolsService) :
    m_toolsService(toolsService)
{
}

void ToolsController::mergePdfs(const QString& firstPath,
                                const QString& secondPath)
{
    m_toolsService->mergePdfs(firstPath, secondPath);
}

}  // namespace adapters::controllers