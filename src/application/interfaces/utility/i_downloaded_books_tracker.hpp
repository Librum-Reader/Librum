#pragma once
#include <vector>
#include <QString>
#include <QUuid>
#include "book.hpp"


namespace application
{

class IDownloadedBooksTracker
{
public:
    virtual ~IDownloadedBooksTracker() noexcept = default;
    
    virtual void setLibraryOwner(const QString& libraryOwnerEmail) = 0;
    virtual std::vector<domain::models::Book> getTrackedBooks() = 0;
    virtual bool trackBook(const domain::models::Book& book) = 0;
    virtual bool untrackBook(const QUuid& uuid) = 0;
    virtual bool updateTrackedBook(const QUuid& uuid,
                                   const domain::models::Book& book) = 0;
};

} // namespace application