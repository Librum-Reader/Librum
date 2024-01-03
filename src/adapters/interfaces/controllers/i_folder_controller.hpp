#pragma once
#include <QObject>
#include "adapters_export.hpp"
#include "folders_proxy_model.hpp"

namespace adapters
{

/**
 * The FolderController class is exposed to the UI code and thus is the
 * "entry point" to the application's backend for folder organization. It acts
 * as a layer of abstraction which maps the user data to a format usable for the
 * application.
 */
class ADAPTERS_EXPORT IFolderController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(adapters::data_models::FoldersProxyModel* foldersModel READ
                   getFoldersModel CONSTANT)

public:
    virtual ~IFolderController() noexcept = default;

    virtual data_models::FoldersProxyModel* getFoldersModel() = 0;

    Q_INVOKABLE virtual bool createFolder(QString name,
                                          QString parent = "") = 0;
    Q_INVOKABLE virtual bool deleteFolder(QString uuid) = 0;
    Q_INVOKABLE virtual void renameFolder(QString uuid, QString newName) = 0;
};

}  // namespace adapters