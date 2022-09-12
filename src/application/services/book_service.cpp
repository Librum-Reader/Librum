#include "book_service.hpp"


namespace application::services
{

using namespace domain::models;

BookService::BookService()
    : m_currentBook(nullptr)
{
    
}

void BookService::addBook(const QString& path)
{
    
}

void BookService::deleteBook(const QString& title)
{
    
}

void BookService::updateBook(const QString& title, const Book& book)
{
    
}

void BookService::addTags(const QString& title, const QList<Tag>& tags)
{
    
}

void BookService::removeTags(const QString& title, const QList<Tag>& tags)
{
    
}

void BookService::books(uint amount) const
{
    
}

void BookService::book(const QString& title) const
{
    
}

bool BookService::setCurrentBook(const QString& title)
{
    
}

const Book& BookService::currentBook() const
{
    
}




} // namespace application::services