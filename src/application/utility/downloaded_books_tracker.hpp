#pragma once
#include "i_downloaded_books_tracker.hpp"


namespace application::utility
{

class DownloadedBooksTracker : public IDownloadedBooksTracker
{
public:
    std::vector<domain::models::Book> getTrackedBooks(const QString& path) override;
    bool trackBook(const QString& path, const domain::models::Book& book) override;
    bool untrackBook(const QString& path, const QUuid& uuid) override;
    bool updateTrackedBook(const QUuid& uuid, const domain::models::Book& book) override;
};

} // namespace application::utility