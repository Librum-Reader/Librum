#include "local_library_tracker.hpp"
#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <iterator>


using namespace domain::entities;

namespace application::utility
{

std::vector<Book> LocalLibraryTracker::getTrackedBooks()
{
    ensureUserLibraryExists();

    QDir libraryDir = getLibraryDir();

    std::vector<Book> books;
    for(auto& metaFileName : libraryDir.entryList(QDir::Files))
    {
        // Skip over files which are not meta files
        if(!metaFileName.endsWith(m_fileExtension))
            continue;

        QFile metaFile(libraryDir.filePath(metaFileName));
        if(!metaFile.open(QFile::ReadOnly | QIODevice::Text))
        {
            qWarning() << QString("Getting tracked book failed. "
                                  "Failed opening " +
                                  m_fileExtension + " file at: %1")
                              .arg(metaFile.fileName());
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

std::optional<Book> LocalLibraryTracker::getTrackedBook(const QUuid& uuid)
{
    ensureUserLibraryExists();

    auto libraryDir = getLibraryDir();
    auto fileName = uuid.toString(QUuid::WithoutBraces);

    QFile metaFile(libraryDir.path() + "/" + fileName + m_fileExtension);
    if(!metaFile.open(QFile::ReadOnly))
    {
        qWarning() << QString("Getting tracked book failed. "
                              "Failed opening .libmeta file at: %1")
                          .arg(metaFile.fileName());
        return std::nullopt;
    }

    auto jsonDoc = QJsonDocument::fromJson(metaFile.readAll());
    auto bookObject = jsonDoc.object();
    auto book = Book::fromJson(bookObject);

    return book;
}

bool LocalLibraryTracker::trackBook(const Book& book)
{
    ensureUserLibraryExists();

    QDir libraryDir = getLibraryDir();
    QFile file(libraryDir.path() + "/" +
               book.getUuid().toString(QUuid::WithoutBraces) + m_fileExtension);

    if(file.exists() || !file.open(QFile::WriteOnly))
    {
        qWarning() << QString("Tracking book failed. "
                              "Failed opening .libmeta file at: %1")
                          .arg(file.fileName());
        return false;
    }

    file.write(book.toJson());
    return true;
}

bool LocalLibraryTracker::untrackBook(const QUuid& uuid)
{
    ensureUserLibraryExists();

    QDir libraryDir = getLibraryDir();
    QString fileToUntrack =
        uuid.toString(QUuid::WithoutBraces) + m_fileExtension;

    auto success = libraryDir.remove(fileToUntrack);
    if(!success)
    {
        qWarning() << QString("Untracking book failed. "
                              "Failed deleting .libmeta file: %1")
                          .arg(fileToUntrack);
    }

    return success;
}

bool LocalLibraryTracker::updateTrackedBook(const Book& book)
{
    ensureUserLibraryExists();

    // Updating is simply deleting the old and creating the new book
    auto untrackingSuccess = untrackBook(book.getUuid());
    if(!untrackingSuccess)
        return false;

    return trackBook(book);
}

void LocalLibraryTracker::saveFolders(const domain::entities::Folder& folder)
{
    ensureUserLibraryExists();

    QDir libraryDir = getLibraryDir();
    QFile file(libraryDir.path() + "/" + m_rootFolderFileName);

    if(!file.open(QFile::WriteOnly))
    {
        qWarning() << QString("Tracking root folder failed. "
                              "Failed opening file at: %1")
                          .arg(file.fileName());
    }

    file.write(folder.toJson());
}

Folder LocalLibraryTracker::loadFolders()
{
    ensureUserLibraryExists();

    QDir libraryDir = getLibraryDir();
    QFile file(libraryDir.path() + "/" + m_rootFolderFileName);

    if(!file.open(QFile::ReadOnly))
    {
        qWarning() << QString("Loading root folder failed. "
                              "Failed opening file at: %1")
                          .arg(file.fileName());
    }

    auto jsonDoc = QJsonDocument::fromJson(file.readAll());
    if(jsonDoc.isEmpty())
        return Folder("invalid", "", "", "");

    auto folderObject = jsonDoc.object();
    auto folder = Folder::fromJson(folderObject, nullptr);

    return folder;
}

void LocalLibraryTracker::setLibraryOwner(const QString& libraryOwnerEmail)
{
    m_libraryOwnerEmail = libraryOwnerEmail;
    m_libraryFolder = getLibraryDir();
}

void LocalLibraryTracker::clearLibraryOwner()
{
    m_libraryOwnerEmail.clear();
    m_libraryFolder = QDir();
}

void LocalLibraryTracker::ensureUserLibraryExists() const
{
    auto libraryDir = getLibraryDir();

    libraryDir.mkpath(libraryDir.path());
}

QJsonDocument LocalLibraryTracker::parseLibMetaFile(QByteArray&& data) const
{
    QJsonParseError parseError;
    auto jsonDoc = QJsonDocument::fromJson(data, &parseError);
    if(parseError.error != QJsonParseError::NoError)
    {
        qWarning() << QString("Error parsing .libmeta file: %1")
                          .arg(parseError.errorString());
    }

    return jsonDoc;
}

QDir LocalLibraryTracker::getLibraryDir() const
{
    QDir destDir(
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if(!destDir.exists())
        destDir.mkpath(".");

    destDir.mkdir("local_libraries");
    destDir.cd("local_libraries");

    auto userLibraryName = getUserLibraryName(m_libraryOwnerEmail);
    destDir.mkdir(userLibraryName);
    destDir.cd(userLibraryName);

    return destDir.path();
}

QString LocalLibraryTracker::getUserLibraryName(QString email) const
{
    // Hash the email to get a unique user library name
    auto hash = qHash(email);

    return QString::number(hash);
}

}  // namespace application::utility
