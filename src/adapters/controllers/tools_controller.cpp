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

    connect(m_toolsService, &application::IToolsService::convertingFinished,
            this, &ToolsController::convertingFinished);
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

void ToolsController::convert(const QString& destName, const QString& format,
                              const QString& filePath)
{
    m_toolsService->convert(destName + format, filePath);
}

}  // namespace adapters::controllers