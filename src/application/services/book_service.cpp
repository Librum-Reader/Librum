#include "book_service.hpp"
#include "book_operation_status.hpp"
#include <algorithm>


namespace application::services
{

using namespace domain::models;


BookService::BookService(IBookInfoManager* bookInfoManager)
    : m_currentBook(nullptr), m_bookInfoManager(bookInfoManager)
{
}


BookOperationStatus BookService::addBook(const QString& filePath)
{
    auto bookPosition = getBookByPath(filePath);
    if(bookPosition != m_books.end())
        return BookOperationStatus::BookAlreadyExists;
    
    QString title = m_bookInfoManager->getBookTitle(filePath);
    m_books.emplace_back(title, filePath);
    
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::deleteBook(const QString& title)
{
    auto bookPosition = getBookByTitle(title);
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
    auto bookPosition = getBookByTitle(title);
    if(bookPosition == m_books.end())
        return BookOperationStatus::BookDoesNotExist;
    
    bookPosition->update(book);
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::addTag(const QString& title,
                                        const domain::models::Tag& tag)
{
    auto bookPosition = getBookByTitle(title);
    if(bookPosition == m_books.end())
        return BookOperationStatus::BookDoesNotExist;
    
    if(!bookPosition->addTag(tag))
        return BookOperationStatus::TagAlreadyExists;
    
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::removeTag(const QString& title,
                                           const domain::models::Tag& tag)
{
    auto bookPosition = getBookByTitle(title);
    if(bookPosition == m_books.end())
        return BookOperationStatus::BookDoesNotExist;
    
    if(!bookPosition->removeTag(tag))
        return BookOperationStatus::TagDoesNotExist;
    
    return BookOperationStatus::Success;
}

const std::vector<Book>& BookService::getAllBooks() const
{
    return m_books;
}

const Book* BookService::getBook(const QString& title) const
{
    auto bookPosition = getBookByTitle(title);
    if(bookPosition == m_books.end())
        return nullptr;
    
    return &(*bookPosition);
}

int BookService::getBookCount() const
{
    return m_books.size();
}

bool BookService::setCurrentBook(const QString& title)
{
    auto bookPosition = getBookByTitle(title);
    if(bookPosition == m_books.end())
        return false;
    
    m_currentBook = &(*bookPosition);
    return true;
}

const Book* BookService::getCurrentBook() const
{
    return m_currentBook;
}


std::vector<Book>::iterator BookService::getBookByTitle(const QString& title)
{
    for(std::size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).title() == title)
            return m_books.begin() + i;
    }
    
    return m_books.end();
}

const std::vector<Book>::const_iterator BookService::getBookByTitle(const QString& title) const
{
    for(std::size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).title() == title)
            return m_books.cbegin() + i;
    }
    
    return m_books.cend();
}

std::vector<Book>::iterator BookService::getBookByPath(const QString& path)
{
    for(std::size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).filePath() == path)
            return m_books.begin() + i;
    }
    
    return m_books.end();
}

} // namespace application::services