#include "book_service.hpp"
#include <QDateTime>
#include <QFile>
#include <QTime>
#include <ranges>
#include "book_operation_status.hpp"
#include "i_book_info_helper.hpp"

#include <QDebug>


namespace application::services
{

using namespace domain::models;
using std::size_t;


BookService::BookService(IBookInfoHelper* bookInfoManager)
    : m_bookInfoManager(bookInfoManager)
{
}


BookOperationStatus BookService::addBook(const QString& filePath)
{
    if(!m_bookInfoManager->setupDocument(filePath, Book::maxCoverWidth, 
                                         Book::maxCoverHeight))
        return BookOperationStatus::OpeningBookFailed;
    
    QString title = m_bookInfoManager->getTitle();
    
    auto book = getBookByTitle(title);
    if(book)
        return BookOperationStatus::BookAlreadyExists;
    
    
    auto author = m_bookInfoManager->getAuthor();
    auto creator = m_bookInfoManager->getCreator();
    auto creationDate = m_bookInfoManager->getCreationDate();
    auto format = m_bookInfoManager->getFormat();
    auto docSize = m_bookInfoManager->getDocumentSize();
    auto pagesSize = m_bookInfoManager->getPagesSize();
    auto pageCount = m_bookInfoManager->getPageCount();
    auto addedToLibrary = getCurrentDateTimeAsString();
    auto lastOpened = "Never";
    
    emit bookInsertionStarted(m_books.size());
    
    m_books.emplace_back(title, author, filePath, creator, creationDate,
                         format, docSize, pagesSize, pageCount, 
                         addedToLibrary, lastOpened);
    
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
    
    auto bookPosition = std::ranges::find_if(m_books, [&title] (const Book& book) {
        return book.getTitle() == title;
    });
    
    size_t index = bookPosition - m_books.begin();
    emit bookDeletionStarted(index);
    m_books.erase(bookPosition);
    emit bookDeletionEnded();
    
    return BookOperationStatus::Success;
}

BookOperationStatus BookService::updateBook(const QString& title,
                                            const Book& newBook)
{
    auto book = getBookByTitle(title);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;
    
    book->update(newBook);
    int index = getBookIndex(newBook.getTitle());
    emit dataChanged(index);
    
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
    
    int index = getBookIndex(title);
    emit tagsChanged(index);
    
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
    
    int index = getBookIndex(title);
    emit tagsChanged(index);
    
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

int BookService::getBookIndex(const QString& title) const
{
    auto* book = getBookByTitle(title);
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

BookOperationStatus BookService::saveBookToPath(const QString& title, 
                                                const QUrl& pathToFolder)
{
    auto book = getBook(title);
    if(!book)
        return BookOperationStatus::BookDoesNotExist;

    QUrl existingBook = book->getFilePath();
    QUrl newBook = pathToFolder.path() + "/" + existingBook.fileName();
    
    auto result = QFile::copy(existingBook.path(), newBook.path());
    if(!result)
    {   
        qDebug() << "Copying book failed!";
        return BookOperationStatus::OperationFailed;
    }
    
    
    return BookOperationStatus::Success;
}

bool BookService::refreshLastOpenedFlag(const QString& title)
{
    auto book = getBookByTitle(title);
    if(!book)
        return false;
    
    auto now = getCurrentDateTimeAsString();
    book->setLastOpened(now);
    
    auto index = getBookIndex(title);
    emit dataChanged(index);
    
    return true;
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
    for(size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).getTitle() == title)
            return &(*(m_books.begin() + i));
    }
    
    return nullptr;
}

const Book* BookService::getBookByTitle(const QString& title) const
{
    for(size_t i = 0; i < m_books.size(); ++i)
    {
        if(m_books.at(i).getTitle() == title)
            return &(*(m_books.cbegin() + i));
    }
    
    return nullptr;
}

QString BookService::getCurrentDateTimeAsString()
{
    auto now = QDateTime::currentDateTimeUtc();
    auto result = now.toString("dd.MM.yyyy") + " - " + now.toString("h:m ap");
    
    return result;
}

} // namespace application::services