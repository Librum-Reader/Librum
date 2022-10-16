#include "book_service.hpp"
#include <QDateTime>
#include <QFile>
#include <QTime>
#include <QDebug>
#include <ranges>
#include "book_operation_status.hpp"
#include "i_book_metadata_helper.hpp"


namespace application::services
{

using namespace domain::models;
using std::size_t;


BookService::BookService(IBookMetadataHelper* bookMetadataHelper)
    : m_bookMetadataHelper(bookMetadataHelper)
{
    connect(m_bookMetadataHelper, &IBookMetadataHelper::bookCoverGenerated,
            this, &BookService::storeBookCover);
}


BookOperationStatus BookService::addBook(const QString& filePath)
{
    auto bookMetaData = m_bookMetadataHelper->getBookMetaData(filePath);
    if(!bookMetaData)
        return BookOperationStatus::OpeningBookFailed;
    
    emit bookInsertionStarted(m_books.size());
    m_books.emplace_back(filePath, bookMetaData.value());
    emit bookInsertionEnded();
    
    // The cover needs to be generated after the book has been created,
    // else the cover is being added to a non existent book
    m_bookMetadataHelper->getCover();
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::deleteBook(const QUuid& uuid)
{
    auto book = getBook(uuid);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;
    
    auto bookPosition = std::ranges::find_if(m_books, [&uuid] (const Book& book) {
        return book.getUuid() == uuid;
    });
    
    size_t index = bookPosition - m_books.begin();
    emit bookDeletionStarted(index);
    m_books.erase(bookPosition);
    emit bookDeletionEnded();
    
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::updateBook(const QUuid& uuid,
                                            const Book& newBook)
{
    auto book = getBook(uuid);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;
    
    book->update(newBook);
    int index = getBookIndex(uuid);
    emit dataChanged(index);
    
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::addTag(const QUuid& uuid,
                                        const domain::models::Tag& tag)
{
    auto book = getBook(uuid);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;
    
    if(!book->addTag(tag))
        return BookOperationStatus::TagAlreadyExists;
    
    int index = getBookIndex(uuid);
    emit tagsChanged(index);
    
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::removeTag(const QUuid& uuid,
                                           const domain::models::Tag& tag)
{
    auto book = getBook(uuid);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;
    
    if(!book->removeTag(tag))
        return BookOperationStatus::TagDoesNotExist;
    
    int index = getBookIndex(uuid);
    emit tagsChanged(index);
    
    return BookOperationStatus::Success;
}

const std::vector<Book>& BookService::getBooks() const
{
    return m_books;
}

const Book* BookService::getBook(const QUuid& uuid) const
{
    for(size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).getUuid() == uuid)
            return &(*(m_books.cbegin() + i));
    }
    
    return nullptr;
}

Book* BookService::getBook(const QUuid& uuid)
{
    for(size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).getUuid() == uuid)
            return &(*(m_books.begin() + i));
    }
    
    return nullptr;
}

int BookService::getBookIndex(const QUuid& uuid) const
{
    auto* book = getBook(uuid);
    if(!book)
        return -1;
    
    std::vector<Book>::const_iterator bookPosition(book);
    size_t index = bookPosition - m_books.begin();
    
    return index;
}

int BookService::getBookCount() const
{
    return m_books.size();
}

BookOperationStatus BookService::saveBookToPath(const QUuid& uuid, 
                                                const QUrl& pathToFolder)
{
    auto book = getBook(uuid);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;

    QUrl existingBook = book->getFilePath();
    QUrl newBook = pathToFolder.path() + "/" + existingBook.fileName();
    
    auto result = QFile::copy(existingBook.path(), newBook.path());
    if(!result)
        return BookOperationStatus::OperationFailed;
    
    
    return BookOperationStatus::Success;
}

bool BookService::refreshLastOpenedFlag(const QUuid& uuid)
{
    auto book = getBook(uuid);
    if(!book)
        return false;
    
    auto now = getCurrentDateTimeAsString();
    book->setLastOpened(now);
    
    auto index = getBookIndex(uuid);
    emit dataChanged(index);
    
    return true;
}

void BookService::setAuthenticationToken(const QString& token)
{
    m_authenticationToken = token;
}

void BookService::clearAuthenticationToken()
{
    m_authenticationToken.clear();
}

void BookService::storeBookCover(const QPixmap* pixmap)
{
    int index = m_books.size() - 1;
    auto& book = m_books.at(index);
    
    book.setCover(pixmap->toImage());
    emit bookCoverGenerated(index);
}


QString BookService::getCurrentDateTimeAsString()
{
    auto now = QDateTime::currentDateTimeUtc();
    auto result = now.toString("dd.MM.yyyy") + " - " + now.toString("h:m ap");
    
    return result;
}

} // namespace application::services