#include "book.hpp"


namespace domain::models
{

Book::Book(QString title)
    : m_title(title)
{
}

QString Book::title()
{
    return m_title;
}

void Book::setTitle(QString newTitle)
{
    m_title = newTitle;
}

    

} // namespace domain::models