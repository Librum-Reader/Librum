#pragma once
#include <QDir>
#include <QString>
#include <QUuid>
#include <optional>
#include <vector>
#include "book.hpp"

namespace application
{

/**
 *  DownloadedBooksTracker maintains a set of files, where each file
 *  contains the data of one currently downloaded book. These files are
 *  used to keep track of books locally, so that "the local library"
 *  (The currently downloaded books) can also be used without an active
 *  internet connection. These local and tracked books, could then be synced
 *  with the server, when an internet connection is established.
 */
class IDownloadedBooksTracker
{
public:
    virtual ~IDownloadedBooksTracker() noexcept = default;

    virtual void setLibraryOwner(const QString& libraryOwnerEmail) = 0;
    virtual QDir getUserLibraryDir() const = 0;

    virtual std::vector<domain::models::Book> getTrackedBooks() = 0;
    virtual std::optional<domain::models::Book> getTrackedBook(
        const QUuid& uuid) = 0;
    virtual bool trackBook(const domain::models::Book& book) = 0;
    virtual bool untrackBook(const QUuid& uuid) = 0;
    virtual bool updateTrackedBook(const domain::models::Book& book) = 0;
};

}  // namespace application