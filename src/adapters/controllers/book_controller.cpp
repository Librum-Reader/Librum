#include "book_controller.hpp"


namespace adapters::controllers
{

BookController::BookController(application::IBookService* bookService)
    : m_bookService(bookService)
{
    
}

void BookController::uploadBook(QString path)
{
    m_bookService->uploadBook(path);
}

void BookController::openBook(QString path)
{
    m_bookService->openBook(path);
}

} // namespace adapters::controllers