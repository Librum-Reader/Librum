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
    virtual domain::entities::Folder* getFolder(const QUuid& uuid) = 0;
    virtual bool createFolder(const QString& name, QString color, QString icon,
                              QString description, const QUuid& parent) = 0;
    virtual bool deleteFolder(const QUuid& uuid) = 0;
    virtual void updateFolder(const domain::entities::Folder& folder) = 0;
    virtual bool moveFolder(const QUuid& uuid, const QUuid& destUuid) = 0;

public slots:
    virtual void setupUserData(const QString& token, const QString& email) = 0;
    virtual void clearUserData() = 0;

signals:
    void beginInsertFolder(domain::entities::Folder* parent, int index);
    void endInsertFolder();
    void beginRemoveFolder(domain::entities::Folder* parent, int index);
    void endRemoveFolder();
    void refreshFolder(domain::entities::Folder* parent, int index);
};

}  // namespace application