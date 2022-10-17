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
    virtual std::vector<domain::models::Book> getTrackedBooks(const QString& path) = 0;
    virtual bool trackBook(const QString& path, const domain::models::Book& book) = 0;
    virtual bool untrackBook(const QString& path, const QUuid& uuid) = 0;
    virtual bool updateTrackedBook(const QUuid& uuid, 
                                   const domain::models::Book& book) = 0;
};

} // namespace application