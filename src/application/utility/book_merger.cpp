#include "book_merger.hpp"

namespace application::utility
{

using domain::entities::Book;

void BookMerger::mergeBooks(Book& localBook, const Book& remoteBook,
                            ILibraryStorageManager* bookStorageManager)
{
    // We are separating the merging process into 3 separate parts, since all of
    // them are independent of each other.
    auto lastOpenedStatus = mergeCurrentPage(localBook, remoteBook);
    auto lastModifiedStatus = mergeBookData(localBook, remoteBook);
    auto coverLastModifiedStatus = mergeBookCover(localBook, remoteBook);


    // Update the local and remote library according to the merge results.
    if(lastOpenedStatus.localLibraryOutdated ||
       lastModifiedStatus.localLibraryOutdated ||
       coverLastModifiedStatus.localLibraryOutdated)
    {
        bookStorageManager->updateBookLocally(localBook);
        emit bookDataChanged(localBook.getUuid());
    }

    if(lastOpenedStatus.remoteLibraryOutdated ||
       lastModifiedStatus.remoteLibraryOutdated ||
       coverLastModifiedStatus.remoteLibraryOutdated)
    {
        bookStorageManager->updateBookRemotely(localBook);
    }

    // Update the book cover
    CoverChanges coverChanges {
        .localBookChanged = coverLastModifiedStatus.localLibraryOutdated,
        .remoteBookChanged = coverLastModifiedStatus.remoteLibraryOutdated,
        .bookUuid = localBook.getUuid(),
        .bookHasCover = localBook.hasCover()
    };
    storeChangesToTheCover(coverChanges, bookStorageManager);
}

MergeStatus BookMerger::mergeCurrentPage(Book& localBook,
                                         const Book& remoteBook)
{
    // Take the current time in seconds, to avoid ms mismatches
    auto localLastOpened = localBook.getLastOpened().toSecsSinceEpoch();
    auto remoteLastOpened = remoteBook.getLastOpened().toSecsSinceEpoch();

    // Exit if there is no difference in the "current page"
    if(remoteLastOpened == localLastOpened)
        return {};

    if(remoteLastOpened > localLastOpened)
    {
        localBook.setCurrentPage(remoteBook.getCurrentPage());
        localBook.setLastOpened(remoteBook.getLastOpened());

        return MergeStatus { .localLibraryOutdated = true };
    }

    return MergeStatus { .remoteLibraryOutdated = true };
}

MergeStatus BookMerger::mergeBookData(Book& localBook, const Book& remoteBook)
{
    // Take the current time in seconds, to avoid ms mismatches
    auto localLastModified = localBook.getLastModified().toSecsSinceEpoch();
    auto remoteLastModified = remoteBook.getLastModified().toSecsSinceEpoch();

    // Exit if there are no differences in their data
    if(remoteLastModified == localLastModified)
        return {};

    if(remoteLastModified > localLastModified)
    {
        auto localBookFilePath = localBook.getFilePath();
        localBook.update(remoteBook);
        localBook.setFilePath(localBookFilePath);

        return MergeStatus { .localLibraryOutdated = true };
    }

    return MergeStatus { .remoteLibraryOutdated = true };
}

MergeStatus BookMerger::mergeBookCover(Book& localBook, const Book& remoteBook)
{
    // Take the current time in seconds, to avoid ms mismatches
    auto localCoverLastModified =
        localBook.getCoverLastModified().toSecsSinceEpoch();
    auto remoteCoverLastModified =
        remoteBook.getCoverLastModified().toSecsSinceEpoch();

    // Exit if there are no cover changes
    if(remoteCoverLastModified == localCoverLastModified)
        return {};

    if(remoteCoverLastModified > localCoverLastModified)
    {
        localBook.setHasCover(remoteBook.hasCover());
        localBook.setCoverLastModified(remoteBook.getCoverLastModified());

        return MergeStatus { .localLibraryOutdated = true };
    }

    return MergeStatus { .remoteLibraryOutdated = true };
}

void BookMerger::storeChangesToTheCover(
    CoverChanges coverChanges, ILibraryStorageManager* bookStorageManager)
{
    if(!coverChanges.newChangesExist())
        return;

    if(coverChanges.remoteBookChanged)
    {
        bookStorageManager->updateBookCoverRemotely(coverChanges.bookUuid,
                                                    coverChanges.bookHasCover);
        return;
    }

    if(coverChanges.bookHasCover)
    {
        bookStorageManager->downloadBookCover(coverChanges.bookUuid);
        return;
    }

    bookStorageManager->deleteBookCoverLocally(coverChanges.bookUuid);
    emit localBookCoverDeleted(coverChanges.bookUuid, "");
}

}  // namespace application::utility