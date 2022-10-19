#pragma once
#include <QDir>
#include "i_downloaded_books_tracker.hpp"


namespace application::utility
{

class DownloadedBooksTracker : public IDownloadedBooksTracker
{
public:
    void setLibraryOwner(const QString& libraryOwnerEmail) override;
    std::vector<domain::models::Book> getTrackedBooks() override;
    std::optional<domain::models::Book> getTrackedBook(const QUuid& uuid) override;
    bool trackBook(const domain::models::Book& book) override;
    bool untrackBook(const QUuid& uuid) override;
    bool updateTrackedBook(const domain::models::Book& book) override;
    
private:
    void ensureUserLibraryExists();
    QDir getUserLibraryDir() const;
    QString getUserLibraryName(QString email) const;
    
    QString m_libraryOwnerEmail;
    QDir m_libraryFolder;
    const QString m_fileExtension = ".libmeta";
};

} // namespace application::utility