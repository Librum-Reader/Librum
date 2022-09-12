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

void BookController::setCurrentBookPath(QString path)
{
    m_currentBookPath = path;
}

QString BookController::currentBookPath()
{
    return m_currentBookPath;
}

} // namespace adapters::controllers