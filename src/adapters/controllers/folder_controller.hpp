#pragma once
#include <QObject>
#include <QString>
#include "adapters_export.hpp"
#include "i_folder_controller.hpp"
#include "i_folder_service.hpp"

namespace adapters::controllers
{

class ADAPTERS_EXPORT FolderController : public IFolderController
{
    Q_OBJECT

public:
    FolderController(application::IFolderService* folderService);

private:
    application::IFolderService* m_folderService;
};

}  // namespace adapters::controllers
