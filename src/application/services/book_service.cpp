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
    auto bookPosition = getBookWithPath(path);
    if(bookPosition != m_books.end())
        return BookOperationStatus::BookAlreadyExists;
    
    QString title = "Some Title";
    m_books.emplace_back(title, path);
    
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::deleteBook(const QString& title)
{
    auto bookPosition = getBookWithTitle(title);
    if(bookPosition == m_books.end())
        return BookOperationStatus::BookDoesNotExist;
    
    if(m_currentBook && m_currentBook->title() == title)
        m_currentBook = nullptr;
    
    m_books.erase(bookPosition);
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::updateBook(const QString& title,
                                            const Book& book)
{
    auto bookPosition = getBookWithTitle(title);
    if(bookPosition == m_books.end())
        return BookOperationStatus::BookDoesNotExist;
    
    bookPosition->update(book);
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::addTags(const QString& title,
                                         const std::vector<Tag>& tags)
{
    auto bookPosition = getBookWithTitle(title);
    if(bookPosition == m_books.end())
        return BookOperationStatus::BookDoesNotExist;
    
    for(const auto& tag : tags)
    {
        bookPosition->addTag(tag);
    }
    
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::removeTags(const QString& title,
                                            const std::vector<Tag>& tags)
{
    auto bookPosition = getBookWithTitle(title);
    if(bookPosition == m_books.end())
        return BookOperationStatus::BookDoesNotExist;
    
    for(const auto& tag : tags)
    {
        bookPosition->removeTag(tag);
    }
    
    return BookOperationStatus::Success;
}

const std::vector<Book>& BookService::books() const
{
    return m_books;
}

const Book* BookService::book(const QString& title) const
{
    auto bookPosition = getBookWithTitle(title);
    if(bookPosition == m_books.end())
        return nullptr;
    
    return &(*bookPosition);
}

bool BookService::setCurrentBook(const QString& title)
{
    auto bookPosition = getBookWithTitle(title);
    if(bookPosition == m_books.end())
        return false;
    
    m_currentBook = &(*bookPosition);
}

const Book* BookService::currentBook() const
{
    return m_currentBook;
}


std::vector<Book>::iterator BookService::getBookWithTitle(const QString& title)
{
    for(std::size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).title() == title)
            return m_books.begin() + i;
    }
    
    return m_books.end();
}

const std::vector<Book>::const_iterator BookService::getBookWithTitle(const QString& title) const
{
    for(std::size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).title() == title)
            return m_books.cbegin() + i;
    }
    
    return m_books.cend();
}

std::vector<Book>::iterator BookService::getBookWithPath(const QString& path)
{
    for(std::size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).filePath() == path)
            return m_books.begin() + i;
    }
    
    return m_books.end();
}

} // namespace application::services