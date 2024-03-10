#include "tools_controller.hpp"

namespace adapters::controllers
{

ToolsController::ToolsController(application::IToolsService* toolsService) :
    m_toolsService(toolsService)
{
    connect(m_toolsService, &application::IToolsService::mergingPdfsFinished,
            this, &ToolsController::mergingPdfsFinished);
}

void ToolsController::mergePdfs(const QString& destName,
                                const QList<QString>& filePaths)
{
    m_toolsService->mergePdfs(destName, filePaths);
}

}  // namespace adapters::controllers