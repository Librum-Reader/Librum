#include "downloaded_books_tracker.hpp"
#include <iterator>
#include <ranges>
#include <QDebug>
#include <QFile>


using namespace domain::models;


namespace application::utility
{

void DownloadedBooksTracker::setLibraryOwner(const QString& libraryOwnerEmail)
{
    m_libraryOwnerEmail = libraryOwnerEmail;
    m_libraryFolder = getUserLibraryDir();
}

std::vector<Book> DownloadedBooksTracker::getTrackedBooks()
{
    ensureUserLibraryExists();
    
    return {};
}

bool DownloadedBooksTracker::trackBook(const Book& book)
{
    ensureUserLibraryExists();
    
    QDir parentDir = getUserLibraryDir();
    QFile file(parentDir.path() + "/" + book.getUuid()
               .toString(QUuid::WithoutBraces) + fileExtension);
    
    if(!file.open(QFile::WriteOnly))
        return false;
    
    file.write(book.serializeToJson());
    return true;
}

bool DownloadedBooksTracker::untrackBook(const QUuid& uuid)
{
    ensureUserLibraryExists();
    
    
}

bool DownloadedBooksTracker::updateTrackedBook(const QUuid& uuid, 
                                               const Book& book)
{
    ensureUserLibraryExists();
}


void DownloadedBooksTracker::ensureUserLibraryExists()
{
    auto libraryDir = getUserLibraryDir();
    
    libraryDir.mkpath(libraryDir.path());
}

QDir DownloadedBooksTracker::getUserLibraryDir() const
{
    auto applicationPath = QDir::current().path();
    auto userLibName = getUserLibraryName(m_libraryOwnerEmail);
    
    auto folderName = applicationPath + "/" + "librum_localLibraries" + "/"  + userLibName;
    return QDir(folderName);
}

QString DownloadedBooksTracker::getUserLibraryName(QString email) const
{
    auto it = std::remove_if(email.begin(), email.end(), [](const QChar& c) { 
        return !c.isLetterOrNumber();
    });
    
    int specialCharCount = std::distance(it, email.end());
    email.chop(specialCharCount);
    
    return email + QString(email.length(), '_');
}

} // namespace application::utility