#pragma once
#include <QDir>
#include <QString>
#include <QUuid>
#include <optional>
#include <vector>
#include "book.hpp"
#include "application_export.hpp"

namespace application
{

/**
 *  LocalLibraryTracker maintains a set of files, where each file
 *  contains the data of one currently downloaded book. These files are
 *  used to keep track of books locally, so that "the local library"
 *  (The currently downloaded books) can also be used without an active
 *  internet connection. These local and tracked books, could then be synced
 *  with the server, when an internet connection is established.
 */
class APPLICATION_EXPORT ILocalLibraryTracker
{
public:
    virtual ~ILocalLibraryTracker() noexcept = default;

    virtual std::vector<domain::entities::Book> getTrackedBooks() = 0;
    virtual std::optional<domain::entities::Book> getTrackedBook(
        const QUuid& uuid) = 0;
    virtual bool trackBook(const domain::entities::Book& book) = 0;
    virtual bool untrackBook(const QUuid& uuid) = 0;
    virtual bool updateTrackedBook(const domain::entities::Book& book) = 0;

    // Setup methods that need to be called first to setup filesystem paths
    virtual void setLibraryOwner(const QString& libraryOwnerEmail) = 0;
    virtual void clearLibraryOwner() = 0;
    virtual QDir getLibraryDir() const = 0;
};

}  // namespace application
