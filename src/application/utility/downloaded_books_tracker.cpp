#include "downloaded_books_tracker.hpp"


namespace application::utility
{

std::vector<domain::models::Book> DownloadedBooksTracker::getTrackedBooks(const QString& path)
{
    
}

bool DownloadedBooksTracker::trackBook(const QString& path, const domain::models::Book& book)
{
    
}

bool DownloadedBooksTracker::untrackBook(const QString& path, const QUuid& uuid)
{
    
}

bool DownloadedBooksTracker::updateTrackedBook(const QUuid& uuid, const domain::models::Book& book)
{
    
}

} // namespace application::utility