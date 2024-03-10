#pragma once
#include "i_library_service.hpp"
#include "i_tools_service.hpp"

namespace application::services
{

class APPLICATION_EXPORT ToolsService : public IToolsService
{
    Q_OBJECT

public:
    ToolsService(ILibraryService* libraryService);

    void mergePdfs(const QString& destName,
                   const QList<QString>& filePaths) override;

private:
    ILibraryService* m_libraryService;
};

}  // namespace application::services