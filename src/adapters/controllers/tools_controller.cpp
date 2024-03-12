#include "tools_controller.hpp"

namespace adapters::controllers
{

ToolsController::ToolsController(application::IToolsService* toolsService) :
    m_toolsService(toolsService)
{
    connect(m_toolsService, &application::IToolsService::mergingFinished, this,
            &ToolsController::mergingFinished);
    connect(m_toolsService,
            &application::IToolsService::extractingPagesFinished, this,
            &ToolsController::extractingPagesFinished);
}

void ToolsController::merge(const QString& destName,
                            const QList<QString>& filePaths)
{
    m_toolsService->merge(destName + ".pdf", filePaths);
}

void ToolsController::extractPages(const QString& destName,
                                   const QString& filePath,
                                   const QString& separatorString)
{
    m_toolsService->extractPages(destName + ".pdf", filePath, separatorString);
}

}  // namespace adapters::controllers