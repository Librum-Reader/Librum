#pragma once
#include <QDir>
#include "i_downloaded_books_tracker.hpp"


namespace application::utility
{

/**
 *  DownloadedBooksTracker maintains a set of files, where each file
 *  contains the data of one currently downloaded book. These files are
 *  used to keep track of books locally, so that "the local library"
 *  (The currently downloaded books) can also be used without an active
 *  internet connection. These local and tracked books, could then be synced
 *  with the server, when an internet connection is established.
 */
class DownloadedBooksTracker : public IDownloadedBooksTracker
{
public:
    void setLibraryOwner(const QString& libraryOwnerEmail) override;
    std::vector<domain::models::Book> getTrackedBooks() override;
    bool trackBook(const domain::models::Book& book) override;
    bool untrackBook(const QUuid& uuid) override;
    bool updateTrackedBook(const QUuid& uuid, 
                           const domain::models::Book& book) override;
    
private:
    void ensureUserLibraryExists();
    QDir getUserLibraryDir() const;
    QString getUserLibraryName(QString email) const;
    
    QString m_libraryOwnerEmail;
    QDir m_libraryFolder;
};

} // namespace application::utility