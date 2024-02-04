#pragma once
#include <QObject>
#include <QString>
#include "application_export.hpp"
#include "folder.hpp"

namespace application
{

/**
 * The FolderGateway class acts as a layer of abstraction before the
 * FolderAccess class. It maps the data provided by the application to
 * the data type required for Folder API requests.
 */
class APPLICATION_EXPORT IFolderStorageGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~IFolderStorageGateway() noexcept = default;

    virtual void updateFolder(const QString& authToken,
                              const domain::entities::Folder& folder) = 0;
    virtual void fetchFolders(const QString& authToken) = 0;

signals:
    void foldersFetched(domain::entities::Folder& folder);
};

}  // namespace application
