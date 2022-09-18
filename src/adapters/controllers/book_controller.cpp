#include "book_controller.hpp"
#include "book_dto.hpp"
#include "book_operation_status.hpp"
#include "tag.hpp"
#include "tag_dto.hpp"
#include <QVariant>


namespace adapters::controllers
{

using namespace domain::models;
using application::BookOperationStatus;

BookController::BookController(application::IBookService* bookService)
    : m_bookService(bookService), m_bookChacheChanged(true), m_currentBookCacheChanged(true)
{
}


BookOperationStatus BookController::addBook(const QString& path)
{
    auto result = m_bookService->addBook(path);
    if(result == BookOperationStatus::Success)
    {
        m_bookChacheChanged = true;
        return BookOperationStatus::Success;
    }
    
    return BookOperationStatus::BookAlreadyExists;
}

BookOperationStatus BookController::deleteBook(const QString& title)
{
    auto result = m_bookService->deleteBook(title);
    if(result == BookOperationStatus::Success)
    {
        if(title == m_currentBookCache.title)
            m_currentBookCacheChanged = true;
        
        m_bookChacheChanged = true;
        return BookOperationStatus::Success;
    }
    
    return BookOperationStatus::BookDoesNotExist;
}

BookOperationStatus BookController::updateBook(const QString& title, const QVariantMap& operations)
{
    auto updateBookPtr = m_bookService->getBook(title);
    if(!updateBookPtr)
        return BookOperationStatus::BookDoesNotExist;
    
    auto updateBook = *updateBookPtr;
    for(auto key : operations.keys())
    {
        if(key == "Title")
        {
            auto title = operations.value("Title");
            updateBook.setTitle(qvariant_cast<QString>(title));
        }
        else if(key == "Cover")
        {
            auto cover = operations.value("Cover");
            updateBook.setTitle(qvariant_cast<QByteArray>(cover));
        }
        else
        {
            return BookOperationStatus::PropertyDoesNotExist;
        }
    }
    
    m_bookChacheChanged = true;
    if(title == m_currentBookCache.title)
        m_currentBookCacheChanged = true;
    
    m_bookService->updateBook(title, updateBook);
    return BookOperationStatus::Success;
}

BookOperationStatus BookController::addTag(const QString& title, const dtos::TagDto& tag)
{
    Tag resultTag(tag.name);
    if(m_bookService->addTag(title, resultTag) == BookOperationStatus::Success)
    {
        m_bookChacheChanged = true;
        if(title == m_currentBookCache.title)
            m_currentBookCacheChanged = true;
        
        return BookOperationStatus::Success;
    }
    
    return BookOperationStatus::TagAlreadyExists;
}

BookOperationStatus BookController::removeTag(const QString& title, const QString& tagName)
{
    Tag resultTag(tagName);
    if(m_bookService->removeTag(title, resultTag) == BookOperationStatus::Success)
    {
        m_bookChacheChanged = true;
        if(title == m_currentBookCache.title)
            m_currentBookCacheChanged = true;
        
        return BookOperationStatus::Success;
    }
    
    return BookOperationStatus::TagDoesNotExist;
}

const dtos::BookDto* BookController::getBook(const QString& title)
{
    if(m_bookChacheChanged)
        refreshBookChache();
    
    const dtos::BookDto* book = getBookFromChache(title);
    return book;
}

int BookController::getBookCount() const
{
    return m_bookService->getBookCount();
}

bool BookController::setCurrentBook(QString title)
{
    if(m_bookService->setCurrentBook(title))
    {
        m_currentBookCacheChanged = true;
        return true;
    }
    
    return false;
}

const dtos::BookDto* BookController::getCurrentBook()
{
    bool currentBookExists = true;
    if(m_currentBookCacheChanged)
        currentBookExists = refreshCurrentBookChache();
    
    if(!currentBookExists)
        return nullptr;
    
    return &m_currentBookCache;
}

void BookController::refreshBookChache()
{
    const auto& books = m_bookService->getAllBooks();
    
    m_bookCache.clear();
    for(std::size_t i = 0; i < books.size(); ++i)
    {
        const auto& book = books[i];
        
        
        dtos::BookDto bookDto;
        bookDto.title = book.title();
        bookDto.filePath = book.filePath();
        bookDto.cover = book.cover();
        
        for(std::size_t k = 0; k < book.tags().size(); ++k)
        {
            dtos::TagDto tagDto;
            tagDto.name = book.tags()[k].name();
            
            bookDto.tags.push_back(tagDto);
        }
        
        
        m_bookCache.emplace_back(std::move(bookDto));
    }
    
    m_bookChacheChanged = false;
}

bool BookController::refreshCurrentBookChache()
{
    const auto* currentBook = m_bookService->getCurrentBook();
    if(!currentBook)
    {
        m_currentBookCacheChanged = false;
        return false;
    }
    
    
    m_currentBookCache.title = currentBook->title();
    m_currentBookCache.filePath = currentBook->filePath();
    m_currentBookCache.cover = currentBook->cover();
    
    m_currentBookCache.tags.clear();
    for(std::size_t i = 0; i < currentBook->tags().size(); ++i)
    {
        dtos::TagDto tagDto;
        tagDto.name = currentBook->tags()[i].name();
        
        m_currentBookCache.tags.append(tagDto);
    }
    
    
    m_currentBookCacheChanged = false;
    return true;
}

dtos::BookDto* BookController::getBookFromChache(const QString& title)
{
    for(std::size_t i = 0; i < m_bookCache.size(); ++i)
    {
        if(m_bookCache[i].title == title)
            return &m_bookCache[i];
    }
    
    return nullptr;
}

} // namespace adapters::controllers