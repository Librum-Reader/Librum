#include "book_controller.hpp"


namespace adapters::controllers
{

using namespace domain::models;

BookController::BookController(application::IBookService* bookService)
    : m_bookService(bookService)
{
}


void BookController::addBook(QString path)
{
    
}

void BookController::deleteBook(QString title)
{
    
}

void BookController::updateBook(QString title, QVariantMap operations)
{
    
}

void BookController::addTags(QString title, QList<QString>)
{
    
}

void BookController::removeTags(QString title, QList<QString>)
{
    
}

void BookController::books(uint amount)
{
    
}

void BookController::book(QString title)
{
    
}

bool BookController::setCurrentBook(QString title)
{
    
}

const Book& BookController::currentBook()
{
    
}

} // namespace adapters::controllers