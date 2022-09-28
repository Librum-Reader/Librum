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
    QString title = m_bookInfoManager->getBookTitle(filePath);
    
    auto book = getBookByTitle(title);
    if(book)
        return BookOperationStatus::BookAlreadyExists;
    
    QByteArray cover = m_bookInfoManager->getBookCover(filePath);
    
    emit bookInsertionStarted(m_books.size());
    m_books.emplace_back(title, filePath, cover);
    emit bookInsertionEnded();
    
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::deleteBook(const QString& title)
{
    auto book = getBookByTitle(title);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;
    
    if(m_currentBook && m_currentBook->title() == title)
        m_currentBook = nullptr;
    
    auto posOfBook = std::find_if(m_books.begin(), m_books.end(), [&title] (const Book& book){
        return book.title() == title;
    });
    
    m_books.erase(posOfBook);
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::updateBook(const QString& title,
                                            const Book& newBook)
{
    auto book = getBookByTitle(title);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;
    
    book->update(newBook);
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::addTag(const QString& title,
                                        const domain::models::Tag& tag)
{
    auto book = getBookByTitle(title);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;
    
    if(!book->addTag(tag))
        return BookOperationStatus::TagAlreadyExists;
    
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::removeTag(const QString& title,
                                           const domain::models::Tag& tag)
{
    auto book = getBookByTitle(title);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;
    
    if(!book->removeTag(tag))
        return BookOperationStatus::TagDoesNotExist;
    
    return BookOperationStatus::Success;
}

const std::vector<Book>& BookService::getBooks() const
{
    return m_books;
}

const Book* BookService::getBook(const QString& title) const
{
    return getBookByTitle(title);
}

int BookService::getBookCount() const
{
    return m_books.size();
}

BookOperationStatus BookService::setCurrentBook(const QString& title)
{
    auto book = getBookByTitle(title);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;
    
    m_currentBook = book;
    return BookOperationStatus::Success;
}

const Book* BookService::getCurrentBook() const
{
    return m_currentBook;
}


Book* BookService::getBookByTitle(const QString& title)
{
    for(std::size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).title() == title)
            return &(*(m_books.begin() + i));
    }
    
    return nullptr;
}

const Book* BookService::getBookByTitle(const QString& title) const
{
    for(std::size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).title() == title)
            return &(*(m_books.cbegin() + i));
    }
    
    return nullptr;
}

} // namespace application::services