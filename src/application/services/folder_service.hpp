#pragma once
#include <QTimer>
#include <memory>
#include "application_export.hpp"
#include "i_folder_service.hpp"
#include "i_folder_storage_gateway.hpp"
#include "i_local_library_tracker.hpp"

namespace application::services
{

class APPLICATION_EXPORT FolderService : public IFolderService
{
    Q_OBJECT

public:
    FolderService(IFolderStorageGateway* folderStorageGateway,
                  ILocalLibraryTracker* localLibraryTracker);

    domain::entities::Folder* getRootFolder() override;
    domain::entities::Folder* getFolder(const QUuid& uuid) override;
    bool createFolder(const QString& name, QString color, QString icon,
                      QString description, const QUuid& parent) override;
    bool deleteFolder(const QUuid& uuid) override;
    void updateFolder(const domain::entities::Folder& folder) override;
    bool moveFolder(const QUuid& uuid, const QUuid& destUuid) override;

public slots:
    void setupUserData(const QString& token, const QString& email) override;
    void clearUserData() override;

private slots:
    void processFetchedFolders(domain::entities::Folder& remoteFolder);

private:
    domain::entities::Folder* getFolderHelper(const QUuid& uuid,
                                              domain::entities::Folder* parent);
    void saveChanges();
    void updateFoldersRecursively(domain::entities::Folder* current,
                                  domain::entities::Folder& remoteFolder);

    IFolderStorageGateway* m_folderStorageGateway;
    ILocalLibraryTracker* m_localLibraryTracker;
    QString m_authenticationToken;

    // This is the root of all folders and will NOT be displayed.
    std::unique_ptr<domain::entities::Folder> m_rootFolder;

    QTimer m_fetchChangesTimer;
    int m_fetchChangesInterval = 200'000;
};

}  // namespace application::services