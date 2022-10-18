#include "book.hpp"
#include <algorithm>
#include <QJsonDocument>
#include <QJsonObject>


namespace domain::models
{

Book::Book(const QString& filePath, const BookMetaData& metaData, 
           int currentPage, const QString& uuid)
    : m_filePath(filePath), m_metaData(metaData), 
      m_currentPage(currentPage)
{
    if(uuid.isEmpty())
        m_uuid = QUuid::createUuid();
    else
        m_uuid = QUuid(uuid);
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


const QString& Book::getAuthor() const
{
    return m_metaData.author;
}

void Book::setAuthor(const QString& newAuthor)
{
    m_metaData.author = newAuthor;
}


const QString& Book::getFilePath() const
{
    return m_filePath;
}

void Book::setFilePath(const QString& newLocalPath)
{
    m_filePath = newLocalPath;
}


const QString& Book::getLastOpened() const
{
    return m_metaData.lastOpened;
}

void Book::setLastOpened(const QString& newLastOpened)
{
    m_metaData.lastOpened = newLastOpened;
}


const QString& Book::getAddedToLibrary() const
{
    return m_metaData.addedToLibrary;
}

void Book::setAddedToLibrary(const QString& newAddedToLibrary)
{
    m_metaData.addedToLibrary = newAddedToLibrary;
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


const QString& Book::getReleaseDate() const
{
    return m_metaData.releaseDate;
}

void Book::setReleaseDate(const QString& newCreationDate)
{
    m_metaData.releaseDate = newCreationDate;
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


const QImage& Book::getCover() const
{
    return m_metaData.cover;
}

void Book::setCover(const QImage& newCover)
{
    m_metaData.cover = newCover;
}


bool Book::getDownloaded() const
{
    return downloaded;
}

void Book::setDownloaded(bool newDownloaded)
{
    downloaded = newDownloaded;
}



const std::vector<Tag>& Book::getTags() const
{
    return m_tags;
}



bool Book::addTag(const Tag& tag)
{
    auto tagPosition = std::find(m_tags.begin(), m_tags.end(), tag);
    if(tagPosition != m_tags.end())
        return false;
    
    m_tags.emplace_back(tag);
    return true;
}

bool Book::removeTag(const Tag& tag)
{
    auto tagPosition = std::find(m_tags.begin(), m_tags.end(), tag);
    if(tagPosition == m_tags.end())
        return false;
    
    m_tags.erase(tagPosition);
    return true;
}


bool Book::tagsAreTheSame(const std::vector<Tag>& other)
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


void Book::update(const Book& other)
{
    if(m_metaData.title != other.getTitle())
        m_metaData.title = other.getTitle();
    if(m_metaData.author != other.getAuthor())
        m_metaData.author = other.getAuthor();
    if(m_filePath != other.getFilePath())
        m_filePath = other.getFilePath();
    if(m_metaData.creator != other.getCreator())
        m_metaData.creator = other.getCreator();
    if(m_metaData.releaseDate != other.getReleaseDate())
        m_metaData.releaseDate = other.getReleaseDate();
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
    if(m_metaData.cover != other.getCover())
        m_metaData.cover = other.getCover();
    
    if(!tagsAreTheSame(other.getTags())) m_tags = other.getTags();
}

QByteArray Book::serializeToJson() const
{
    QJsonObject book
    {
        {"uuid", getUuid().toString(QUuid::WithoutBraces)},
        {"title", getTitle()},
        {"author", getAuthor()},
        {"creator", getCreator()},
        {"pageCount", getPageCount()},
        {"currentPage", getCurrentPage()},
        {"releaseDate", getReleaseDate()},
        {"format", getFormat()},
        {"language", getLanguage()},
        {"documentSize", getDocumentSize()},
        {"pagesSize", getPagesSize()},
        {"addedToLibrary", getAddedToLibrary()},
        {"lastOpened", getLastOpened()},
        {"filePath", getFilePath()}/*,*/
//        {"cover", getCover().bytes}
    };
    
    QJsonDocument doc(book);
    QString strJson(doc.toJson(QJsonDocument::Indented));
    
    return strJson.toUtf8();
}

} // namespace domain::models