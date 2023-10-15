#include "book.hpp"
#include <QBuffer>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <algorithm>
#include <cmath>
#include <ranges>

using namespace domain::value_objects;

namespace domain::entities
{

Book::Book(const QString& filePath, const BookMetaData& metaData,
           int currentPage, const QString& uuid) :
    m_metaData(metaData),
    m_filePath(filePath),
    m_currentPage(currentPage)
{
    if(uuid.isEmpty())
        m_uuid = QUuid::createUuid();
    else
        m_uuid = QUuid(uuid);
}

bool Book::operator==(const Book& rhs) const
{
    bool dataIsTheSame = m_uuid == rhs.m_uuid && m_filePath == rhs.m_filePath &&
                         m_isDownloaded == rhs.m_isDownloaded &&
                         m_currentPage == rhs.m_currentPage;
    bool tagsAreTheSame = rhs.tagsAreTheSame(m_tags);
    bool highlightsAreTheSame = rhs.highlightsAreTheSame(m_highlights);
    bool bookmarksAreTheSame = rhs.bookmarksAreTheSame(m_bookmarks);

    return tagsAreTheSame && highlightsAreTheSame && dataIsTheSame &&
           bookmarksAreTheSame && m_metaData == rhs.m_metaData;
}

const QUuid& Book::getUuid() const
{
    return m_uuid;
}

const QString& Book::getTitle() const
{
    return m_metaData.title;
}

void Book::setTitle(const QString& newTitle)
{
    m_metaData.title = newTitle;
}

const QString& Book::getAuthors() const
{
    return m_metaData.authors;
}

void Book::setAuthors(const QString& newAuthor)
{
    m_metaData.authors = newAuthor;
}

const QString& Book::getFilePath() const
{
    return m_filePath;
}

void Book::setFilePath(const QString& newLocalPath)
{
    m_filePath = newLocalPath;
}

bool Book::hasCover() const
{
    return m_metaData.hasCover;
}

void Book::setHasCover(bool hasCover)
{
    m_metaData.hasCover = hasCover;
}

const QString& Book::getCoverPath() const
{
    return m_metaData.coverPath;
}

void Book::setCoverPath(const QString& path)
{
    m_metaData.coverPath = path;
}

double Book::getMediaDownloadProgress() const
{
    return m_metaData.bookMediaDownloadProgress;
}

void Book::setMediaDownloadProgress(double newProgress)
{
    m_metaData.bookMediaDownloadProgress = newProgress;
}

const QList<Highlight>& Book::getHighlights() const
{
    return m_highlights;
}

void Book::setHighlights(QList<Highlight>&& highlights)
{
    m_highlights = std::move(highlights);
}

void Book::addHighlight(const Highlight& highlight)
{
    m_highlights.append(highlight);
}

void Book::changeHighlightColor(const QUuid& uuid, const QColor& newColor)
{
    auto highlight =
        std::ranges::find_if(m_highlights,
                             [&uuid](const Highlight& highlight)
                             {
                                 return highlight.getUuid() == uuid;
                             });

    if(highlight != m_highlights.end())
        highlight->setColor(newColor);
}

void Book::removeHighlight(QUuid uuid)
{
    m_highlights.removeIf(
        [&uuid](const Highlight& highlight)
        {
            return highlight.getUuid() == uuid;
        });
}

const QList<Bookmark>& Book::getBookmarks() const
{
    return m_bookmarks;
}

void Book::setBookmarks(QList<Bookmark>&& bookmarks)
{
    m_bookmarks = std::move(bookmarks);
}

void Book::addBookmark(const Bookmark& bookmark)
{
    m_bookmarks.append(bookmark);
}

void Book::renameBookmark(const QUuid& uuid, const QString& newName)
{
    for(auto& bookmark : m_bookmarks)
    {
        if(bookmark.getUuid() == uuid)
            bookmark.setName(newName);
    }
}

void Book::removeBookmark(QUuid uuid)
{
    m_bookmarks.removeIf(
        [&uuid](const Bookmark& bookmark)
        {
            return bookmark.getUuid() == uuid;
        });
}

const QDateTime& Book::getCoverLastModified() const
{
    return m_metaData.coverLastModified;
}

void Book::updateCoverLastModified()
{
    m_metaData.coverLastModified = QDateTime::currentDateTimeUtc();
}

void Book::setCoverLastModified(const QDateTime& newTime)
{
    m_metaData.coverLastModified = newTime;
}

const QDateTime& Book::getLastOpened() const
{
    return m_metaData.lastOpened;
}

void Book::setLastOpened(const QDateTime& newLastOpened)
{
    m_metaData.lastOpened = newLastOpened;
}

void Book::updateLastOpened()
{
    m_metaData.lastOpened = QDateTime::currentDateTimeUtc();
}

const QDateTime& Book::getAddedToLibrary() const
{
    return m_metaData.addedToLibrary;
}

void Book::setAddedToLibrary(const QDateTime& newAddedToLibrary)
{
    m_metaData.addedToLibrary = newAddedToLibrary;
}

const QDateTime& Book::getLastModified() const
{
    return m_metaData.lastModified;
}

void Book::setLastModified(const QDateTime& newLastModified)
{
    m_metaData.lastModified = newLastModified;
}

void Book::updateLastModified()
{
    m_metaData.lastModified = QDateTime::currentDateTimeUtc();
}

const QString& Book::getPagesSize() const
{
    return m_metaData.pagesSize;
}

void Book::setPagesSize(const QString& newPagesSize)
{
    m_metaData.pagesSize = newPagesSize;
}

const QString& Book::getDocumentSize() const
{
    return m_metaData.documentSize;
}

void Book::setDocumentSize(const QString& newDocumentSize)
{
    m_metaData.documentSize = newDocumentSize;
}

const QString& Book::getFormat() const
{
    return m_metaData.format;
}

void Book::setFormat(const QString& newFormat)
{
    m_metaData.format = newFormat;
}

const QString& Book::getLanguage() const
{
    return m_metaData.language;
}

void Book::setLanguage(const QString& newLanguage)
{
    m_metaData.language = newLanguage;
}

const QString& Book::getCreationDate() const
{
    return m_metaData.creationDate;
}

void Book::setCreationDate(const QString& newCreationDate)
{
    m_metaData.creationDate = newCreationDate;
}

int Book::getPageCount() const
{
    return m_metaData.pageCount;
}

void Book::setPageCount(int newPageCount)
{
    m_metaData.pageCount = newPageCount;
}

int Book::getCurrentPage() const
{
    return m_currentPage;
}

void Book::setCurrentPage(int newCurrentPage)
{
    m_currentPage = newCurrentPage;
}

const QString& Book::getCreator() const
{
    return m_metaData.creator;
}

void Book::setCreator(const QString& newCreator)
{
    m_metaData.creator = newCreator;
}

bool Book::isDownloaded() const
{
    return m_isDownloaded;
}

void Book::setDownloaded(bool newDownloaded)
{
    m_isDownloaded = newDownloaded;
}

bool Book::existsOnlyOnClient() const
{
    return m_existsOnlyOnClient;
}

void Book::setExistsOnlyOnClient(bool newExistsOnlyOnClient)
{
    m_existsOnlyOnClient = newExistsOnlyOnClient;
}

int Book::getBookReadingProgress() const
{
    if(!getLastOpened().isValid() ||
       (getPageCount() > getCurrentPage() && getCurrentPage() <= 1))
    {
        return 0;
    }

    double pageCountAsDouble = static_cast<double>(getPageCount());
    double percentageInDecimal = (getCurrentPage()) / pageCountAsDouble;

    return std::round(percentageInDecimal * 100);
}

const QList<Tag>& Book::getTags() const
{
    return m_tags;
}

bool Book::addTag(const Tag& tag)
{
    if(getTagByUuid(tag.getUuid()) != nullptr)
        return false;

    m_tags.emplace_back(tag);
    return true;
}

bool Book::removeTag(const QUuid& uuid)
{
    auto tagPosition = std::ranges::find_if(m_tags,
                                            [uuid](const Tag& tag)
                                            {
                                                return tag.getUuid() == uuid;
                                            });
    if(tagPosition == m_tags.end())
        return false;

    m_tags.erase(tagPosition);
    return true;
}

bool Book::renameTag(const QUuid& uuid, const QString& newName)
{
    auto* tag = getTagByUuid(uuid);
    if(tag == nullptr)
        return false;

    if(getTagByName(newName) != nullptr)
        return false;

    tag->setName(newName);
    return true;
}

bool Book::tagsAreTheSame(const QList<Tag>& other) const
{
    if(m_tags.size() != other.size())
        return false;

    for(int i = 0; i < m_tags.size(); ++i)
    {
        if(m_tags.at(i) != other.at(i))
            return false;
    }

    return true;
}

bool Book::highlightsAreTheSame(const QList<Highlight>& other) const
{
    if(m_highlights.size() != other.size())
        return false;

    for(int i = 0; i < m_highlights.size(); ++i)
    {
        if(m_highlights.at(i) != other.at(i))
            return false;
    }

    return true;
}

bool Book::bookmarksAreTheSame(const QList<Bookmark>& other) const
{
    if(m_bookmarks.size() != other.size())
        return false;

    for(int i = 0; i < m_bookmarks.size(); ++i)
    {
        if(m_bookmarks.at(i) != other.at(i))
            return false;
    }

    return true;
}

Tag* Book::getTagByUuid(const QUuid& uuid)
{
    auto tagPosition = std::ranges::find_if(m_tags,
                                            [uuid](const Tag& tag)
                                            {
                                                return tag.getUuid() == uuid;
                                            });
    if(tagPosition == m_tags.end())
        return nullptr;

    return &(*tagPosition);
}

Tag* Book::getTagByName(const QString& name)
{
    auto tagPosition = std::ranges::find_if(m_tags,
                                            [name](const Tag& tag)
                                            {
                                                return tag.getName() == name;
                                            });
    if(tagPosition == m_tags.end())
        return nullptr;

    return &(*tagPosition);
}

void Book::update(const Book& other)
{
    if(m_metaData.title != other.getTitle())
        m_metaData.title = other.getTitle();
    if(m_metaData.authors != other.getAuthors())
        m_metaData.authors = other.getAuthors();
    if(m_filePath != other.getFilePath())
        m_filePath = other.getFilePath();
    if(m_metaData.creator != other.getCreator())
        m_metaData.creator = other.getCreator();
    if(m_metaData.creationDate != other.getCreationDate())
        m_metaData.creationDate = other.getCreationDate();
    if(m_metaData.format != other.getFormat())
        m_metaData.format = other.getFormat();
    if(m_metaData.language != other.getLanguage())
        m_metaData.language = other.getLanguage();
    if(m_metaData.documentSize != other.getDocumentSize())
        m_metaData.documentSize = other.getDocumentSize();
    if(m_metaData.pagesSize != other.getPagesSize())
        m_metaData.pagesSize = other.getPagesSize();
    if(m_metaData.pageCount != other.getPageCount())
        m_metaData.pageCount = other.getPageCount();
    if(m_metaData.addedToLibrary != other.getAddedToLibrary())
        m_metaData.addedToLibrary = other.getAddedToLibrary();
    if(m_metaData.lastOpened != other.getLastOpened())
        m_metaData.lastOpened = other.getLastOpened();
    if(m_metaData.lastModified != other.getLastModified())
        m_metaData.lastModified = other.getLastModified();
    if(m_metaData.coverLastModified != other.getCoverLastModified())
        m_metaData.coverLastModified = other.getCoverLastModified();
    if(m_metaData.hasCover != other.hasCover())
        m_metaData.hasCover = other.hasCover();
    if(m_metaData.coverPath != other.getCoverPath())
        m_metaData.coverPath = other.getCoverPath();

    if(!tagsAreTheSame(other.getTags()))
        m_tags = other.getTags();
    if(!highlightsAreTheSame(other.getHighlights()))
        m_highlights = other.getHighlights();
    if(!bookmarksAreTheSame(other.getBookmarks()))
        m_bookmarks = other.getBookmarks();
}

bool Book::isValid() const
{
    // Required
    int titleSize = m_metaData.title.size();
    int authorsSize = m_metaData.authors.size();
    int creatorSize = m_metaData.creator.size();
    int creationDateSize = m_metaData.creationDate.size();


    return (titleSize >= 2 && titleSize <= 200) && authorsSize <= 400 &&
           creatorSize <= 140 && creationDateSize <= 140;
}

long Book::getSizeInBytes() const
{
    return getBytesFromSizeString(m_metaData.documentSize) +
           getCoverSizeInBytes();
}

// An example input for this function would be 24 MiB. This function converts x
// Mib, Bytes or KiB to its corresponding amount of bytes.
long Book::getBytesFromSizeString(QString size) const
{
    // Make sure there are no spaces and decimal points are dots '.' so that we
    // can parse the size correctly, independent of the culture.
    size = size.replace(" ", "");
    size = size.replace(",", ".");

    auto dataPair = splitSizeStringInNumbersAndFormat(size);
    auto numbers = dataPair.first;
    auto type = dataPair.second;

    if(type.toLower() == "b")
        return numbers;
    if(type.toLower() == "kb")
        return numbers * 1000;
    if(type.toLower() == "mb")
        return numbers * 1000 * 1000;
    if(type.toLower() == "gb")
        return numbers * 1000 * 1000 * 1000;

    qWarning() << "Tried getting size in bytes from a book with a "
                  "non-supported size type: "
               << type;
    return 0;
}

QPair<long, QString> Book::splitSizeStringInNumbersAndFormat(
    const QString& sizeString) const
{
    int typeBegining = -1;
    for(int i = 0; i < sizeString.length(); i++)
    {
        if(!sizeString[i].isDigit() && sizeString[i] != '.')
        {
            typeBegining = i;
            break;
        }
    }

    auto numbers = sizeString.mid(0, typeBegining).toLong();
    auto type = sizeString.mid(typeBegining);
    return { numbers, type };
}

long Book::getCoverSizeInBytes() const
{
    if(!m_metaData.hasCover || m_metaData.coverPath.isEmpty())
        return 0;

    QFile cover(m_metaData.coverPath);
    if(!cover.open(QFile::ReadOnly))
        return 0;

    return cover.size();
}

QByteArray Book::toJson() const
{
    QJsonObject book {
        { "uuid", getUuid().toString(QUuid::WithoutBraces) },
        { "title", getTitle() },
        { "authors", getAuthors() },
        { "creator", getCreator() },
        { "pageCount", getPageCount() },
        { "currentPage", getCurrentPage() },
        { "creationDate", getCreationDate() },
        { "format", getFormat() },
        { "language", getLanguage() },
        { "documentSize", getDocumentSize() },
        { "pagesSize", getPagesSize() },
        { "addedToLibrary",
          getAddedToLibrary().toString(dateTimeStringFormat) },
        { "lastOpened", getLastOpened().toString(dateTimeStringFormat) },
        { "lastModified", getLastModified().toString(dateTimeStringFormat) },
        { "filePath", getFilePath() },
        { "coverLastModified",
          getCoverLastModified().toString(dateTimeStringFormat) },
        { "hasCover", hasCover() },
        { "coverPath", getCoverPath() },
        { "existsOnlyOnClient", existsOnlyOnClient() },
        { "tags", serializeTags() },
        { "highlights", serializeHighlights() },
        { "bookmarks", serializeBookmarks() },
    };

    QJsonDocument doc(book);
    QString strJson = doc.toJson(QJsonDocument::Indented);

    return strJson.toUtf8();
}

QJsonArray Book::serializeTags() const
{
    QJsonArray tags;
    for(const auto& tag : m_tags)
    {
        auto obj = QJsonDocument::fromJson(tag.toJson()).object();
        tags.append(QJsonValue::fromVariant(obj));
    }

    return tags;
}

QJsonArray Book::serializeHighlights() const
{
    QJsonArray highlights;
    for(const auto& highlight : m_highlights)
    {
        auto obj = QJsonDocument::fromJson(highlight.toJson()).object();
        highlights.append(QJsonValue::fromVariant(obj));
    }

    return highlights;
}

QJsonArray Book::serializeBookmarks() const
{
    QJsonArray bookmarks;
    for(const auto& bookmark : m_bookmarks)
    {
        auto obj = QJsonDocument::fromJson(bookmark.toJson()).object();
        bookmarks.append(QJsonValue::fromVariant(obj));
    }

    return bookmarks;
}

Book Book::fromJson(const QJsonObject& jsonBook)
{
    BookMetaData metaData = getBookMetaDataFromJson(jsonBook);
    QString filePath = jsonBook["filePath"].toString();
    int currentPage = jsonBook["currentPage"].toInt();
    QString uuid = jsonBook["uuid"].toString();
    bool existsOnlyOnClient = jsonBook["existsOnlyOnClient"].toBool(false);

    Book book(filePath, metaData, currentPage, uuid);
    book.setExistsOnlyOnClient(existsOnlyOnClient);
    addTagsToBook(book, jsonBook["tags"].toArray());
    addHighlightsToBook(book, jsonBook["highlights"].toArray());
    addBookmarksToBook(book, jsonBook["bookmarks"].toArray());

    return book;
}

BookMetaData Book::getBookMetaDataFromJson(const QJsonObject& jsonBook)
{
    BookMetaData metaData {
        .title = jsonBook["title"].toString(),
        .authors = jsonBook["authors"].toString(),
        .creator = jsonBook["creator"].toString(),
        .creationDate = jsonBook["creationDate"].toString(),
        .format = jsonBook["format"].toString(),
        .language = jsonBook["language"].toString(),
        .documentSize = jsonBook["documentSize"].toString(),
        .pagesSize = jsonBook["pagesSize"].toString(),
        .pageCount = jsonBook["pageCount"].toInt(),
        .addedToLibrary = QDateTime::fromString(
            jsonBook["addedToLibrary"].toString(), dateTimeStringFormat),
        .lastModified = QDateTime::fromString(
            jsonBook["lastModified"].toString(), dateTimeStringFormat),
        .lastOpened = QDateTime::fromString(jsonBook["lastOpened"].toString(),
                                            dateTimeStringFormat),
        .coverLastModified = QDateTime::fromString(
            jsonBook["coverLastModified"].toString(), dateTimeStringFormat),
        .hasCover = jsonBook["hasCover"].toBool(),
        .coverPath = jsonBook["coverPath"].toString(),
    };

    // Specify that the dates are UTC, else Qt thinks its local time
    metaData.addedToLibrary.setTimeSpec(Qt::UTC);
    metaData.lastModified.setTimeSpec(Qt::UTC);
    metaData.lastOpened.setTimeSpec(Qt::UTC);
    metaData.coverLastModified.setTimeSpec(Qt::UTC);

    return metaData;
}

void Book::addTagsToBook(Book& book, const QJsonArray& jsonTags)
{
    for(const auto& jsonTag : jsonTags)
    {
        auto tagObject = jsonTag.toObject();
        Tag tag = Tag::fromJson(tagObject);

        book.addTag(tag);
    }
}

void Book::addHighlightsToBook(Book& book, const QJsonArray& jsonHighlights)
{
    for(const auto& jsonHighlight : jsonHighlights)
    {
        auto highlightObject = jsonHighlight.toObject();
        Highlight highlight = Highlight::fromJson(highlightObject);

        book.addHighlight(highlight);
    }
}

void Book::addBookmarksToBook(Book& book, const QJsonArray& jsonBookmarks)
{
    for(const auto& jsonBookmark : jsonBookmarks)
    {
        auto bookmarkObject = jsonBookmark.toObject();
        auto bookmark = Bookmark::fromJson(bookmarkObject);

        book.addBookmark(bookmark);
    }
}

}  // namespace domain::entities