#include "book_controller.hpp"
#include <QVariant>
#include "book_dto.hpp"
#include "book_operation_status.hpp"
#include "tag.hpp"
#include "tag_dto.hpp"


namespace adapters::controllers
{

using namespace domain::models;
using application::BookOperationStatus;

BookController::BookController(application::IBookService* bookService)
    : m_bookChacheChanged(true), m_currentBookCacheChanged(true), 
      m_bookService(bookService), m_libraryModel(m_bookService->getBooks())
{
    QObject::connect(m_bookService, &application::IBookService::bookInsertionStarted,
                     &m_libraryModel, &data_models::LibraryModel::beginInsertingRow);
    QObject::connect(m_bookService, &application::IBookService::bookInsertionEnded,
                     &m_libraryModel, &data_models::LibraryModel::endInsertingRow);
}


int BookController::addBook(const QString& path)
{
    auto result = m_bookService->addBook(path);
    if(result == BookOperationStatus::Success)
    {
        m_bookChacheChanged = true;
        return static_cast<int>(BookOperationStatus::Success);
    }
    
    return static_cast<int>(BookOperationStatus::BookAlreadyExists);
}

int BookController::deleteBook(const QString& title)
{
    auto result = m_bookService->deleteBook(title);
    if(result == BookOperationStatus::Success)
    {
        m_bookChacheChanged = true;
        if(title == m_currentBookCache.title)
            m_currentBookCacheChanged = true;
        
        return static_cast<int>(BookOperationStatus::Success);
    }
    
    return static_cast<int>(BookOperationStatus::BookDoesNotExist);
}

int BookController::updateBook(const QString& title, const QVariantMap& operations)
{
    auto bookToUpdate = m_bookService->getBook(title);
    if(!bookToUpdate)
        return static_cast<int>(BookOperationStatus::BookDoesNotExist);
    
    auto updatedBook = *bookToUpdate;
    for(auto key : operations.keys())
    {
        if(key == "Title")
        {
            auto title = operations.value("Title");
            updatedBook.setTitle(qvariant_cast<QString>(title));
        }
        else if(key == "Cover")
        {
            auto cover = operations.value("Cover");
            updatedBook.setTitle(qvariant_cast<QByteArray>(cover));
        }
        else
        {
            return static_cast<int>(BookOperationStatus::PropertyDoesNotExist);
        }
    }
    
    m_bookChacheChanged = true;
    if(title == m_currentBookCache.title)
        m_currentBookCacheChanged = true;
    
    m_bookService->updateBook(title, updatedBook);
    return static_cast<int>(BookOperationStatus::Success);
}

int BookController::addTag(const QString& title, const dtos::TagDto& tag)
{
    Tag tagToAdd(tag.name);
    if(m_bookService->addTag(title, tagToAdd) == BookOperationStatus::Success)
    {
        m_bookChacheChanged = true;
        if(title == m_currentBookCache.title)
            m_currentBookCacheChanged = true;
        
        return static_cast<int>(BookOperationStatus::Success);
    }
    
    return static_cast<int>(BookOperationStatus::TagAlreadyExists);
}

int BookController::removeTag(const QString& title, const QString& tagName)
{
    Tag tagToRemove(tagName);
    if(m_bookService->removeTag(title, tagToRemove) == BookOperationStatus::Success)
    {
        m_bookChacheChanged = true;
        if(title == m_currentBookCache.title)
            m_currentBookCacheChanged = true;
        
        return static_cast<int>(BookOperationStatus::Success);
    }
    
    return static_cast<int>(BookOperationStatus::TagDoesNotExist);
}

dtos::BookDto BookController::getBook(const QString& title)
{
    if(m_bookChacheChanged)
        refreshBookChache();
    
    return *getBookFromChache(title);
}

int BookController::getBookCount() const
{
    return m_bookService->getBookCount();
}

int BookController::setCurrentBook(QString title)
{
    auto result = m_bookService->setCurrentBook(title);
    if(result == BookOperationStatus::Success)
        m_currentBookCacheChanged = true;
    
    return static_cast<int>(result);
}

dtos::BookDto BookController::getCurrentBook()
{
    // TODO: Check if the current file does not exist
    bool currentBookExists = true;
    if(m_currentBookCacheChanged)
        currentBookExists = refreshCurrentBookChache();
    
    return m_currentBookCache;
}


data_models::LibraryModel* BookController::getLibraryModel()
{
    return &m_libraryModel;
}


void BookController::refreshBookChache()
{
    const auto& books = m_bookService->getBooks();
    
    m_bookCache.clear();
    for(const auto& book : books)
    {
        dtos::BookDto bookDto;
        bookDto.title = book.title();
        bookDto.filePath = book.filePath();
        bookDto.cover = book.cover();
        
        for(std::size_t i = 0; i < book.tags().size(); ++i)
        {
            dtos::TagDto tagDto;
            tagDto.name = book.tags()[i].name();
            
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
    for(const Tag& tag : currentBook->tags())
    {
        dtos::TagDto tagDto;
        tagDto.name = tag.name();
        
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