#include "downloaded_books_tracker.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <iterator>
#include <QDebug>
#include <QFile>
#include "book_meta_data.hpp"


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
    
    QDir libraryDir = getUserLibraryDir();
    
    std::vector<Book> books;
    for(const QString& metaFileName : libraryDir.entryList(QDir::Files))
    {
        QFile metaFile(libraryDir.path() + "/" + metaFileName);
        if(!metaFile.open(QFile::ReadOnly))
            return {};
        
        auto jsonDoc = QJsonDocument::fromJson(metaFile.readAll());
        auto bookObject = jsonDoc.object();
        
        
        BookMetaData metaData
        {
            .title = bookObject["title"].toString(),
            .author = bookObject["author"].toString(),
            .creator = bookObject["creator"].toString(),
            .releaseDate = bookObject["releaseDate"].toString(),
            .format = bookObject["format"].toString(),
            .language = bookObject["language"].toString(),
            .documentSize = bookObject["documentSize"].toString(),
            .pagesSize = bookObject["pagesSize"].toString(),
            .pageCount = bookObject["pageCount"].toInt(),
            .addedToLibrary = bookObject["addedToLibrary"].toString(),
            .lastOpened = bookObject["lastOpened"].toString()
        };
        
        auto cover = bookObject["cover"].toString();
        auto coverWithoutType = cover.mid(22, -1);
        metaData.cover = QImage::fromData(QByteArray::fromBase64(coverWithoutType.toUtf8()));
        
        
        QString filePath = bookObject["filePath"].toString();
        int currentPage = bookObject["currentPage"].toInt();
        QString uuid = bookObject["uuid"].toString();
        books.emplace_back(filePath, metaData, currentPage, uuid);
    }
    
    return books;
}

bool DownloadedBooksTracker::trackBook(const Book& book)
{
    ensureUserLibraryExists();
    
    QDir libraryDir = getUserLibraryDir();
    QFile file(libraryDir.path() + "/" + book.getUuid()
               .toString(QUuid::WithoutBraces) + m_fileExtension);
    
    if(!file.open(QFile::WriteOnly))
        return false;
    
    file.write(book.serializeToJson());
    return true;
}

bool DownloadedBooksTracker::untrackBook(const QUuid& uuid)
{
    ensureUserLibraryExists();
    
    QDir libraryDir = getUserLibraryDir();
    QString fileToUntrack = uuid.toString(QUuid::WithoutBraces) + m_fileExtension;
    
    auto success = libraryDir.remove(fileToUntrack);
    
    return success;
}

bool DownloadedBooksTracker::updateTrackedBook(const Book& book)
{
    ensureUserLibraryExists();
    
    untrackBook(book.getUuid());
    trackBook(book);
    
    return true;
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