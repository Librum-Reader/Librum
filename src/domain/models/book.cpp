#include "book.hpp"
#include <algorithm>


namespace domain::models
{

Book::Book(const QString& title, const QString& author, 
           const QString& filePath, const QImage& cover,
           const QString& uuid)
    : m_title(title), m_author(author), m_filePath(filePath), m_cover(cover)
{
    if(uuid.isEmpty())
        m_uuid = QUuid::createUuid();
    else
        m_uuid = QUuid(uuid);
}

Book::Book(const QString& title, const QString& author, 
           const QString& filePath, const QString& creator,
           const QString& creationDate, const QString& format,
           const QString& docSize, const QString& pagesSize,
           int pageCount, const QString& addedToLibrary,
           const QString& lastOpened, const QImage& cover,
           int currentPage, const QString& uuid)
    : m_title(title), m_author(author), m_filePath(filePath), m_creator(creator),
      m_releaseDate(creationDate), m_format(format), m_documentSize(docSize),
      m_pagesSize(pagesSize), m_addedToLibrary(addedToLibrary), 
      m_pageCount(pageCount), m_lastOpened(lastOpened), 
      m_cover(cover), m_currentPage(currentPage)
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
    return m_title;
}

void Book::setTitle(const QString& newTitle)
{
    m_title = newTitle;
}


const QString& Book::getAuthor() const
{
    return m_author;
}

void Book::setAuthor(const QString& newAuthor)
{
    m_author = newAuthor;
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
    return m_lastOpened;
}

void Book::setLastOpened(const QString& newLastModified)
{
    m_lastOpened = newLastModified;
}


const QString& Book::getAddedToLibrary() const
{
    return m_addedToLibrary;
}

void Book::setAddedToLibrary(const QString& newAddedToLibrary)
{
    m_addedToLibrary = newAddedToLibrary;
}


const QString& Book::getPagesSize() const
{
    return m_pagesSize;
}

void Book::setPagesSize(const QString& newPagesSize)
{
    m_pagesSize = newPagesSize;
}


const QString& Book::getDocumentSize() const
{
    return m_documentSize;
}

void Book::setDocumentSize(const QString& newDocumentSize)
{
    m_documentSize = newDocumentSize;
}


const QString& Book::getFormat() const
{
    return m_format;
}

void Book::setFormat(const QString& newFormat)
{
    m_format = newFormat;
}


const QString& Book::getLanguage() const
{
    return m_language;
}

void Book::setLanguage(const QString& newLanguage)
{
    m_language = newLanguage;
}


const QString& Book::getReleaseDate() const
{
    return m_releaseDate;
}

void Book::setReleaseDate(const QString& newCreationDate)
{
    m_releaseDate = newCreationDate;
}


int Book::getPageCount() const
{
    return m_pageCount;
}

void Book::setPageCount(int newPageCount)
{
    m_pageCount = newPageCount;
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
    return m_creator;
}

void Book::setCreator(const QString& newCreator)
{
    m_creator = newCreator;
}


const QImage& Book::getCover() const
{
    return m_cover;
}

void Book::setCover(const QImage& newCover)
{
    m_cover = newCover;
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
    if(m_title != other.getTitle()) m_title = other.getTitle();
    if(m_author != other.getAuthor()) m_author = other.getAuthor();
    if(m_filePath != other.getFilePath()) m_filePath = other.getFilePath();
    if(m_creator != other.getCreator()) m_creator = other.getCreator();
    if(m_releaseDate != other.getReleaseDate()) m_releaseDate = other.getReleaseDate();
    if(m_format != other.getFormat()) m_format = other.getFormat();
    if(m_language != other.getLanguage()) m_language = other.getLanguage();
    if(m_documentSize != other.getDocumentSize()) m_documentSize = other.getDocumentSize();
    if(m_pagesSize != other.getPagesSize()) m_pagesSize = other.getPagesSize();
    if(m_pageCount != other.getPageCount()) m_pageCount = other.getPageCount();
    if(m_addedToLibrary != other.getAddedToLibrary()) m_addedToLibrary = other.getAddedToLibrary();
    if(m_lastOpened != other.getLastOpened()) m_lastOpened = other.getLastOpened();
    if(m_cover != other.getCover()) m_cover = other.getCover();
    if(!tagsAreTheSame(other.getTags())) m_tags = other.getTags();
}

} // namespace domain::models