#include "book.hpp"
#include <algorithm>


namespace domain::models
{

Book::Book(const QString& title, const QString& author, 
           const QString& filePath, const QImage& cover)
    : m_title(title), m_author(author), m_filePath(filePath), m_cover(cover)
{
}

Book::Book(const QString& title, const QString& author, 
           const QString& filePath, const QString& creator,
           const QString& creationDate, const QString& format,
           const QString& docSize, const QString& pagesSize,
           int pageCount, const QString& addedToLibrary,
           const QString& lastModified, const QImage& cover)
    : m_title(title), m_author(author), m_filePath(filePath), m_creator(creator),
      m_creationDate(creationDate), m_format(format), m_documentSize(docSize),
      m_pagesSize(pagesSize), m_addedToLibrary(addedToLibrary), 
      m_pageCount(pageCount), m_lastModified(lastModified), m_cover(cover)
{
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


const QString& Book::getLastModified() const
{
    return m_lastModified;
}

void Book::setLastModified(const QString& newLastModified)
{
    m_lastModified = newLastModified;
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


const QString& Book::getCreationDate() const
{
    return m_creationDate;
}

void Book::setCreationDate(const QString& newCreationDate)
{
    m_creationDate = newCreationDate;
}


int Book::getPageCount() const
{
    return m_pageCount;
}

void Book::setPageCount(int newPageCount)
{
    m_pageCount = newPageCount;
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
    if(m_filePath != other.getFilePath()) m_filePath = other.getFilePath();
    if(m_cover != other.getCover()) m_cover = other.getCover();
    if(!tagsAreTheSame(other.getTags())) m_tags = other.getTags();
}

} // namespace domain::models