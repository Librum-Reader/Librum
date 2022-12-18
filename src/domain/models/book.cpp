#include "book.hpp"
#include <QBuffer>
#include <QJsonArray>
#include <QJsonDocument>
#include <algorithm>
#include <cmath>
#include <ranges>

namespace domain::models
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
                         m_downloaded == rhs.m_downloaded &&
                         m_currentPage == rhs.m_currentPage;

    return dataIsTheSame && m_metaData == rhs.m_metaData;
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

const QDateTime& Book::getLastOpened() const
{
    return m_metaData.lastOpened;
}

void Book::setLastOpened(const QDateTime& newLastOpened)
{
    m_metaData.lastOpened = newLastOpened;
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

QImage Book::getCover() const
{
    return m_metaData.cover;
}

QString Book::getCoverAsString() const
{
    if(m_metaData.cover.isNull())
        return QString("");

    QByteArray byteArray;
    QBuffer buffer(&byteArray);

    buffer.open(QIODevice::WriteOnly);
    m_metaData.cover.save(&buffer, "png");
    auto result = QString::fromUtf8(byteArray.toBase64());

    return result;
}

QString Book::getCoverAsStringWithType() const
{
    if(m_metaData.cover.isNull())
        return QString("");

    QByteArray byteArray;
    QBuffer buffer(&byteArray);

    buffer.open(QIODevice::WriteOnly);
    m_metaData.cover.save(&buffer, "png");
    QString base64 = QString::fromUtf8(byteArray.toBase64());

    return QString("data:image/png;base64,") + base64;
}

void Book::setCover(const QImage& newCover)
{
    m_metaData.cover = newCover;
}

bool Book::getDownloaded() const
{
    return m_downloaded;
}

void Book::setDownloaded(bool newDownloaded)
{
    m_downloaded = newDownloaded;
}

int Book::getBookProgressPercentage() const
{
    if(!getLastOpened().isValid() ||
       (getPageCount() > 1 && getCurrentPage() == 0))
    {
        return 0;
    }

    double pageCountAsDouble = static_cast<double>(getPageCount());
    double percentageInDecimal = (getCurrentPage() + 1) / pageCountAsDouble;

    return std::round(percentageInDecimal * 100);
}

const std::vector<Tag>& Book::getTags() const
{
    return m_tags;
}

bool Book::addTag(const Tag& tag)
{
    auto existingTag = getTagByUuid(tag.getUuid());
    if(existingTag != nullptr)
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
    auto tag = getTagByUuid(uuid);
    if(tag == nullptr)
        return false;

    if(getTagByName(newName) != nullptr)
        return false;

    tag->setName(newName);
    return true;
}

bool Book::tagsAreTheSame(const std::vector<Tag>& other) const
{
    if(m_tags.size() != other.size())
        return false;

    for(std::size_t i = 0; i < m_tags.size(); ++i)
    {
        if(m_tags.at(i) != other.at(i))
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
    if(m_metaData.cover != other.getCover())
        m_metaData.cover = other.getCover();

    if(!tagsAreTheSame(other.getTags()))
        m_tags = other.getTags();
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
        { "cover", getCoverAsString() },
        { "tags", serializeTags() },
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

Book Book::fromJson(const QJsonObject& jsonBook)
{
    BookMetaData metaData = getBookMetaDataFromJson(jsonBook);
    QString filePath = jsonBook["filePath"].toString();
    int currentPage = jsonBook["currentPage"].toInt();
    QString uuid = jsonBook["uuid"].toString();

    Book book(filePath, metaData, currentPage, uuid);
    addTagsToBook(book, jsonBook["tags"].toArray());

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
        .cover = getBookCoverFromJson(jsonBook),
    };

    // Specify that the dates are UTC, else Qt thinks its local time
    metaData.addedToLibrary.setTimeSpec(Qt::UTC);
    metaData.lastModified.setTimeSpec(Qt::UTC);
    metaData.lastOpened.setTimeSpec(Qt::UTC);

    return metaData;
}

QImage Book::getBookCoverFromJson(const QJsonObject& jsonBook)
{
    auto cover = jsonBook["cover"].toString();
    return QImage::fromData(QByteArray::fromBase64(cover.toUtf8()));
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

}  // namespace domain::models