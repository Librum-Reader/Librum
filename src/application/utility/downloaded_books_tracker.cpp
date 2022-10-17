#include "downloaded_books_tracker.hpp"
#include <QDir>


using namespace domain::models;


namespace application::utility
{

std::vector<Book> DownloadedBooksTracker::getTrackedBooks(const QString& folder)
{
    
}

bool DownloadedBooksTracker::trackBook(const QString& folder, const Book& book)
{
    
}

bool DownloadedBooksTracker::untrackBook(const QString& folder, const QUuid& uuid)
{
    
}

bool DownloadedBooksTracker::updateTrackedBook(const QUuid& uuid, const Book& book)
{
    
}

} // namespace application::utility