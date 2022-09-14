#include "book_service.hpp"
#include "book_operation_staus.hpp"
#include <algorithm>


namespace application::services
{

using namespace domain::models;


BookService::BookService()
    : m_currentBook(nullptr)
{
}


BookOperationStatus BookService::addBook(const QString& path)
{
    auto bookPosition = bookWithPathExists(path);
    if(bookPosition != m_books.end())
        return BookOperationStatus::BookAlreadyExists;
    
    QString title = "Some Title";
    m_books.emplace_back(title, path);
    
    return BookOperationStatus::Success;
}

std::vector<Book>::iterator BookService::bookWithPathExists(const QString& path)
{
    for(std::size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).filePath() == path)
            return m_books.begin() + i;
    }
    
    return m_books.end();
}


BookOperationStatus BookService::deleteBook(const QString& title)
{
    auto bookPosition = bookWithTitleExists(title);
    if(bookPosition == m_books.end())
        return BookOperationStatus::BookDoesNotExist;
    
    if(m_currentBook && m_currentBook->title() == title)
        m_currentBook = nullptr;
    
    m_books.erase(bookPosition);
    return BookOperationStatus::Success;
}

std::vector<Book>::iterator BookService::bookWithTitleExists(const QString& title)
{
    for(std::size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).title() == title)
            return m_books.begin() + i;
    }
    
    return m_books.end();
}


BookOperationStatus BookService::updateBook(const QString& title,
                                            const Book& book)
{
    
}

BookOperationStatus BookService::addTags(const QString& title,
                                         const std::vector<Tag>& tags)
{
    
}

BookOperationStatus BookService::removeTags(const QString& title,
                                            const std::vector<Tag>& tags)
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