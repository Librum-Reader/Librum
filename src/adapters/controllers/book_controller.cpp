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

} // namespace adapters::controllers