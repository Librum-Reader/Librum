#include "book_metadata_helper.hpp"
#include <QDateTime>
#include <QDebug>
#include <QMimeDatabase>
#include "book.hpp"


using namespace domain::value_objects;
using namespace domain::entities;

namespace application::utility
{


std::optional<BookMetaData> BookMetadataHelper::getBookMetaData(
    const QString& filePath)
{
    BookMetaData metaData {
        .title = getTitle(filePath),
        .authors = getAuthors(),
        .creator = getCreator(),
        .creationDate = getCreationDate(),
        .format = getFormat(),
        .language = "",
        .documentSize = getDocumentSize(),
        .pagesSize = getPagesSize(),
        .pageCount = getPageCount(),
        .lastOpened = QDateTime(),
    };

    return metaData;
}

bool BookMetadataHelper::setupDocument(const QString& filePath)
{
    return true;
}

void BookMetadataHelper::setupDocumentObserver()
{
}

QString BookMetadataHelper::getTitle(const QString& filePath) const
{
    return "Test";
}

QString BookMetadataHelper::getAuthors() const
{
    return "Test";
}

QString BookMetadataHelper::getCreator() const
{
    return "Test";
}

QString BookMetadataHelper::getCreationDate() const
{
    return "Test";
}

QString BookMetadataHelper::getFormat() const
{
    return "Test";
}

QString BookMetadataHelper::getDocumentSize() const
{
    return "Test";
}

QString BookMetadataHelper::getPagesSize() const
{
    return "Test";
}

int BookMetadataHelper::getPageCount() const
{
    return 0;
}

void BookMetadataHelper::loadCover() const
{
}

QSize BookMetadataHelper::getCoverSize() const
{
    return QSize();
}

QString BookMetadataHelper::getSystemRelativePath(const QString& qPath) const
{
    QString prefix = "file://";
    return qPath.mid(prefix.size());
}

QMimeType BookMetadataHelper::getMimeType(const QString& filePath)
{
    QMimeDatabase mimeDb;
    auto mimeType = mimeDb.mimeTypeForUrl(filePath);

    return mimeType;
}

QString BookMetadataHelper::getTitleFromBookPath(const QString& path) const
{
    auto indexOfLastSlash = path.lastIndexOf("/");
    auto indexOfLastDot = path.lastIndexOf(".");

    if(indexOfLastDot == -1)
        return path.mid(indexOfLastSlash + 1);

    auto result =
        path.mid(indexOfLastSlash + 1, indexOfLastDot - indexOfLastSlash - 1);

    return result;
}

QString BookMetadataHelper::getFileTypeFromPath(const QString& path) const
{
    // We want to get the file type from the path, so from: some/path.pdf
    // we want "pdf". We do this by taking everthing after the last dot.
    int lastPositionOfDot = path.lastIndexOf(".");
    if(lastPositionOfDot == -1)
        return "pdf";  // The default is pdf

    auto result = path.mid(lastPositionOfDot + 1);
    return result;
}

QString BookMetadataHelper::removeSuffixFromMimeString(
    const QString& mimeString) const
{
    // The suffix in "text/font+woff" is "+woff" which can be removed by
    // cutting everything off after the '+'
    int lastPositionOfPlus = mimeString.lastIndexOf("+");
    if(lastPositionOfPlus == -1)
        return mimeString;

    auto result = mimeString.mid(0, lastPositionOfPlus);
    return result;
}

void BookMetadataHelper::proccessCoverPixmap(int page, int flag)
{
}

}  // namespace application::utility