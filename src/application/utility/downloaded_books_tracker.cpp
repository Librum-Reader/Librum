#include "downloaded_books_tracker.hpp"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <iterator>


using namespace domain::entities;

namespace application::utility
{

std::vector<Book> DownloadedBooksTracker::getTrackedBooks()
{
    ensureUserLibraryExists();

    QDir libraryDir = getLibraryDir();

    std::vector<Book> books;
    for(auto& metaFileName : libraryDir.entryList(QDir::Files))
    {
        QFile metaFile(libraryDir.filePath(metaFileName));
        if(!metaFile.open(QFile::ReadOnly | QIODevice::Text))
        {
            qWarning() << "Failed opening .libmeta file at: "
                       << metaFile.fileName()
                       << " for operation 'getTrackedBooks'";
            continue;
        }

        QByteArray jsonData = metaFile.readAll();
        auto jsonDoc = parseLibMetaFile(std::move(jsonData));

        auto bookObject = jsonDoc.object();
        auto book = Book::fromJson(bookObject);

        books.emplace_back(std::move(book));
    }

    return books;
}

std::optional<Book> DownloadedBooksTracker::getTrackedBook(const QUuid& uuid)
{
    ensureUserLibraryExists();

    auto libraryDir = getLibraryDir();
    auto fileName = uuid.toString(QUuid::WithoutBraces);

    QFile metaFile(libraryDir.path() + "/" + fileName + m_fileExtension);
    if(!metaFile.open(QFile::ReadOnly))
    {
        qWarning() << "Failed opening .libmeta file at: " << metaFile.fileName()
                   << " for operation 'getTrackedBook'";
        return std::nullopt;
    }

    auto jsonDoc = QJsonDocument::fromJson(metaFile.readAll());
    auto bookObject = jsonDoc.object();
    auto book = Book::fromJson(bookObject);

    return book;
}

bool DownloadedBooksTracker::trackBook(const Book& book)
{
    ensureUserLibraryExists();

    QDir libraryDir = getLibraryDir();
    QFile file(libraryDir.path() + "/" +
               book.getUuid().toString(QUuid::WithoutBraces) + m_fileExtension);

    if(file.exists() || !file.open(QFile::WriteOnly))
    {
        qWarning() << "Failed opening .libmeta file at: " << file.fileName()
                   << " for operation 'trackBook'";
        return false;
    }

    file.write(book.toJson());
    return true;
}

bool DownloadedBooksTracker::untrackBook(const QUuid& uuid)
{
    ensureUserLibraryExists();

    QDir libraryDir = getLibraryDir();
    QString fileToUntrack =
        uuid.toString(QUuid::WithoutBraces) + m_fileExtension;

    auto success = libraryDir.remove(fileToUntrack);
    if(!success)
    {
        qWarning() << "Failed removing .libmeta file called: " << fileToUntrack
                   << " for operation 'untrackBook'";
    }

    return success;
}

bool DownloadedBooksTracker::updateTrackedBook(const Book& book)
{
    ensureUserLibraryExists();

    // Updating is simply deleting the old and creating the new book
    auto untrackingSuccess = untrackBook(book.getUuid());
    if(!untrackingSuccess)
        return false;

    return trackBook(book);
}

void DownloadedBooksTracker::setLibraryOwner(const QString& libraryOwnerEmail)
{
    m_libraryOwnerEmail = libraryOwnerEmail;
    m_libraryFolder = getLibraryDir();
}

void DownloadedBooksTracker::clearLibraryOwner()
{
    m_libraryOwnerEmail.clear();
    m_libraryFolder = QDir();
}

void DownloadedBooksTracker::ensureUserLibraryExists()
{
    auto libraryDir = getLibraryDir();

    libraryDir.mkpath(libraryDir.path());
}

QJsonDocument DownloadedBooksTracker::parseLibMetaFile(QByteArray&& data) const
{
    QJsonParseError parseError;
    auto jsonDoc = QJsonDocument::fromJson(data, &parseError);
    if(parseError.error != QJsonParseError::NoError)
    {
        qWarning() << "Error parsing .libmeta file:"
                   << parseError.errorString();
    }

    return jsonDoc;
}

QDir DownloadedBooksTracker::getLibraryDir() const
{
    auto applicationPath = QDir::current().path();
    auto userLibName = getUserLibraryName(m_libraryOwnerEmail);

    auto uniqueFolderName =
        applicationPath + "/" + "librum_localLibraries" + "/" + userLibName;

    return QDir(uniqueFolderName);
}

QString DownloadedBooksTracker::getUserLibraryName(QString email) const
{
    // The user library name is the user's email without special characters,
    // followed by '_' repeated x times, where x is the length of the email.
    auto emailWithoutSpecialChars = removeSpecialCharacters(email);

    return emailWithoutSpecialChars + QString(email.length(), '_');
}

QString DownloadedBooksTracker::removeSpecialCharacters(QString& str) const
{
    auto it = std::remove_if(str.begin(), str.end(),
                             [](const QChar& c)
                             {
                                 return !c.isLetterOrNumber();
                             });

    int specialCharCount = std::distance(it, str.end());
    return str.chopped(specialCharCount);
}

}  // namespace application::utility