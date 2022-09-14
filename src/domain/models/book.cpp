#include "book.hpp"
#include <algorithm>


namespace domain::models
{

Book::Book(QString title, QString path, QByteArray cover)
    : m_title(title), m_filePath(path), m_cover(cover)
{
}


const QString& Book::title() const
{
    return m_title;
}

void Book::setTitle(const QString& newTitle)
{
    m_title = newTitle;
}

const QString& Book::filePath() const
{
    return m_filePath;
}

void Book::setFilePath(const QString& newLocalPath)
{
    m_filePath = newLocalPath;
}

const QByteArray& Book::cover() const
{
    return m_cover;
}

void Book::setCover(const QByteArray& newCover)
{
    m_cover = newCover;
}

const std::vector<Tag>& Book::tags()
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
    

} // namespace domain::models