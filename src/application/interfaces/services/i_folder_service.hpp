#pragma once
#include <QObject>
#include <QString>
#include "application_export.hpp"
#include "folder.hpp"

namespace application
{

/**
 *  The FolderService handles all Folder operations, including storage etc.
 */
class APPLICATION_EXPORT IFolderService : public QObject
{
    Q_OBJECT

public:
    virtual ~IFolderService() noexcept = default;

    virtual domain::entities::Folder* getRootFolder() = 0;
    virtual bool createFolder(QString name, QUuid parent) = 0;
    virtual bool deleteFolder(QUuid uuid) = 0;
    virtual void renameFolder(QUuid uuid, QString newName) = 0;

signals:
    void beginInsertFolder(domain::entities::Folder* parent, int index);
    void endInsertFolder();
    void beginRemoveFolder(domain::entities::Folder* parent, int index);
    void endRemoveFolder();
    void refreshFolder(domain::entities::Folder* parent, int index);
};

}  // namespace application
