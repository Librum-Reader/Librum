#pragma once
#include <QObject>
#include <QString>
#include <icon_proxy_model.hpp>
#include "adapters_export.hpp"
#include "folders_model.hpp"
#include "i_folder_controller.hpp"
#include "i_folder_service.hpp"
#include "icon_model.hpp"

namespace adapters::controllers
{

class ADAPTERS_EXPORT FolderController : public IFolderController
{
    Q_OBJECT

public:
    FolderController(application::IFolderService* folderService);

    data_models::FoldersProxyModel* getFoldersModel() override;
    data_models::IconProxyModel* getIconModel() override;

    dtos::FolderDto getFolder(QString uuid) override;
    bool createFolder(QString name, QString color, QString icon,
                      QString description, QString parent = "") override;
    bool deleteFolder(QString uuid) override;
    void updateFolder(QString uuid, QString name, QString color, QString icon,
                      QString description) override;
    bool moveFolder(QString uuid, QString destUuid) override;

private:
    application::IFolderService* m_folderService;

    data_models::FoldersModel m_foldersModel;
    data_models::FoldersProxyModel m_foldersProxyModel;

    data_models::IconModel m_iconModel;
    data_models::IconProxyModel m_iconProxyModel;
};

}  // namespace adapters::controllers