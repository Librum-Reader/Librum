#include "book_merger.hpp"

namespace application::utility
{

using domain::entities::Book;

void BookMerger::mergeBooks(Book& localBook, const Book& remoteBook,
                            IBookStorageManager* bookStorageManager)
{
    // We are separating the merging process into 3 separate parts, since all of
    // them are independent of each other.
    auto lastOpenedStatus = mergeCurrentPage(localBook, remoteBook);
    auto lastModifiedStatus = mergeBookData(localBook, remoteBook);
    auto coverLastModifiedStatus = mergeBookCover(localBook, remoteBook);

    // Update the local and remote library according to the merge results.
    // Update the local library when 'localLibraryOutdated' == true, vice versa.
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
    // Take the current time in seconds, so that there are no ms mismatches
    auto localLastOpened = localBook.getLastOpened().toSecsSinceEpoch();
    auto remoteLastOpened = remoteBook.getLastOpened().toSecsSinceEpoch();

    // There are no "current page" differences between the local and remote
    // book
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
    // Take the current time in seconds, so that there are no ms mismatches
    auto localLastModified = localBook.getLastModified().toSecsSinceEpoch();
    auto remoteLastModified = remoteBook.getLastModified().toSecsSinceEpoch();

    // There are no data differences between the local and remote book
    if(remoteLastModified == localLastModified)
        return {};

    if(remoteLastModified > localLastModified)
    {
        // Save the file path since its overwritten during the update
        // operation
        auto localBookFilePath = localBook.getFilePath();
        localBook.update(remoteBook);
        localBook.setFilePath(localBookFilePath);

        return MergeStatus { .localLibraryOutdated = true };
    }

    return MergeStatus { .remoteLibraryOutdated = true };
}

MergeStatus BookMerger::mergeBookCover(Book& localBook, const Book& remoteBook)
{
    // Take the current time in seconds, so that there are no ms mismatches
    auto localCoverLastModified =
        localBook.getCoverLastModified().toSecsSinceEpoch();
    auto remoteCoverLastModified =
        remoteBook.getCoverLastModified().toSecsSinceEpoch();


    // There are no differences between the local and remote cover
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

void BookMerger::storeChangesToTheCover(CoverChanges coverChanges,
                                        IBookStorageManager* bookStorageManager)
{
    // Exit if there are no new changes
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
        bookStorageManager->getCoverForBook(coverChanges.bookUuid);
        return;
    }

    bookStorageManager->deleteBookCoverLocally(coverChanges.bookUuid);
    emit localBookCoverDeleted(coverChanges.bookUuid, "");
}

}  // namespace application::utility