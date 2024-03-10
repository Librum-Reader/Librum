#pragma once
#include <QObject>
#include "adapters_export.hpp"
#include "i_tools_controller.hpp"
#include "i_tools_service.hpp"

namespace adapters::controllers
{

class ADAPTERS_EXPORT ToolsController : public IToolsController
{
    Q_OBJECT

public:
    ToolsController(application::IToolsService* toolsService);

    void mergePdfs(const QList<QString>& filePaths) override;

private:
    application::IToolsService* m_toolsService;
};

}  // namespace adapters::controllers
