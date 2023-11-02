#pragma once
#include <QObject>
#include "application_export.hpp"
#include "book.hpp"
#include "i_library_storage_manager.hpp"
#include "merge_status.hpp"

namespace application::utility
{

/**
 * This class merges two books by comparing them and then updating the
 * appropriate one, locally or remotely.
 */
class APPLICATION_EXPORT BookMerger : public QObject
{
    Q_OBJECT

public:
    void mergeBooks(domain::entities::Book& localBook,
                    const domain::entities::Book& remoteBook,
                    ILibraryStorageManager* bookStorageManager);

signals:
    void localBookCoverDeleted(const QUuid& uuid, const QString& emptyPath);
    void bookDataChanged(const QUuid& uuid);

private:
    struct CoverChanges
    {
        bool localBookChanged;
        bool remoteBookChanged;
        QUuid bookUuid;
        bool bookHasCover;

        bool newChangesExist() const
        {
            return localBookChanged || remoteBookChanged;
        };
    };

private:
    MergeStatus mergeCurrentPage(domain::entities::Book& localBook,
                                 const domain::entities::Book& remoteBook);
    MergeStatus mergeBookData(domain::entities::Book& localBook,
                              const domain::entities::Book& remoteBook);
    MergeStatus mergeBookCover(domain::entities::Book& localBook,
                               const domain::entities::Book& remoteBook);
    void storeChangesToTheCover(CoverChanges coverChanges,
                                ILibraryStorageManager* bookStorageManager);
};

}  // namespace application::utility