#pragma once
#include <QObject>
#include <QString>
#include "adapters_export.hpp"
#include "folders_model.hpp"
#include "i_folder_controller.hpp"
#include "i_folder_service.hpp"

namespace adapters::controllers
{

class ADAPTERS_EXPORT FolderController : public IFolderController
{
    Q_OBJECT

public:
    FolderController(application::IFolderService* folderService);

    data_models::FoldersProxyModel* getFoldersModel() override;
    dtos::FolderDto getFolder(QString uuid) override;
    bool createFolder(QString name, QString parent = "") override;
    bool deleteFolder(QString uuid) override;
    void updateFolder(QString uuid, QString name, QString icon,
                      QString description) override;

private:
    application::IFolderService* m_folderService;
    data_models::FoldersModel m_foldersModel;
    data_models::FoldersProxyModel m_foldersProxyModel;
};

}  // namespace adapters::controllers