#include "downloaded_books_tracker.hpp"


using namespace domain::models;


namespace application::utility
{

std::vector<Book> DownloadedBooksTracker::getTrackedBooks(const QString& path)
{
    
}

bool DownloadedBooksTracker::trackBook(const QString& path, const Book& book)
{
    
}

bool DownloadedBooksTracker::untrackBook(const QString& path, const QUuid& uuid)
{
    
}

bool DownloadedBooksTracker::updateTrackedBook(const QUuid& uuid, const Book& book)
{
    
}

} // namespace application::utility