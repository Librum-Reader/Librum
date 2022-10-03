#include "book_service.hpp"
#include "book_operation_status.hpp"
#include "i_book_info_helper.hpp"
#include <algorithm>


namespace application::services
{

using namespace domain::models;


BookService::BookService(IBookInfoHelper* bookInfoManager)
    : m_bookInfoManager(bookInfoManager), m_currentBook(nullptr)
{
}


BookOperationStatus BookService::addBook(const QString& filePath)
{
    if(!m_bookInfoManager->setupDocument(filePath))
        return BookOperationStatus::OpeningBookFailed;
    
    QString title = m_bookInfoManager->getTitle();
    QString author = m_bookInfoManager->getAuthor();
    QString authorResult = author == "" ? 
                               "Unknown Author" : author;
    
    auto book = getBookByTitle(title);
    if(book)
        return BookOperationStatus::BookAlreadyExists;
    
    
    emit bookInsertionStarted(m_books.size());
    m_books.emplace_back(title, authorResult, filePath);
    emit bookInsertionEnded();
    
    
    QObject::connect(m_bookInfoManager, &IBookInfoHelper::bookCoverGenerated,
                     this, &BookService::storeBookCover);
    m_bookInfoManager->getCover();
    
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

void BookService::storeBookCover(const QPixmap* pixmap)
{
    int index = m_books.size() - 1;
    auto& book = m_books.at(index);
    
    book.setCover(pixmap->toImage());
    emit bookCoverGenerated(index);
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