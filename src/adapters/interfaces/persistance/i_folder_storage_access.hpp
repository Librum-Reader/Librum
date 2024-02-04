#pragma once
#include <QByteArray>
#include <QObject>
#include <QString>
#include "adapters_export.hpp"

namespace adapters
{

/**
 * The FolderStorageAccess class makes the API calls to the API of the used
 * Folder to store folders, etc. from it.
 */
class ADAPTERS_EXPORT IFolderStorageAccess : public QObject
{
    Q_OBJECT

public:
    virtual ~IFolderStorageAccess() noexcept = default;

    virtual void updateFolder(const QString& authToken,
                              const QByteArray& folder) = 0;
    virtual void fetchFolders(const QString& authToken) = 0;

signals:
    void foldersFetched(const QByteArray& folders);
    void noFolderExistsForUser();
};

}  // namespace adapters
