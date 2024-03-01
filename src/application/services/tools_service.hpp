#pragma once
#include "i_tools_service.hpp"

namespace application::services
{

class APPLICATION_EXPORT ToolsService : public IToolsService
{
    Q_OBJECT

public:
    void mergePdfs(const QString& firstPath,
                   const QString& secondPath) override;
};

}  // namespace application::services