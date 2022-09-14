#include "book.hpp"


namespace domain::models
{

Book::Book(QString title)
    : m_title(title)
{
}


const QString& Book::title()
{
    return m_title;
}

void Book::setTitle(const QString& newTitle)
{
    m_title = newTitle;
}

const QString& Book::localPath() const
{
    return m_localPath;
}

void Book::setLocalPath(const QString& newLocalPath)
{
    m_localPath = newLocalPath;
}

const QByteArray& Book::cover() const
{
    return m_cover;
}

void Book::setCover(const QByteArray& newCover)
{
    m_cover = newCover;
}

const QList<Tag>& Book::tags()
{
    return m_tags;
}

bool Book::addTag(const Tag& tag)
{
    auto containsTag = m_tags.contains(tag);
    if(containsTag)
        return false;
    
    m_tags.push_back(tag);
    return true;
}

bool Book::removeTag(const Tag& tag)
{
    auto index = m_tags.indexOf(tag);
    if(index == -1)
        return false;
    
    m_tags.removeAt(index);
    return true;
}
    

} // namespace domain::models