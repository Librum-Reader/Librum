#pragma once
#include "i_tools_service.hpp"

namespace application::services
{

class APPLICATION_EXPORT ToolsService : public IToolsService
{
    Q_OBJECT

public:
    void mergePdfs(const QList<QString>& filePaths) override;
};

}  // namespace application::services