#include "book_controller.hpp"
#include <QUuid>

namespace adapters::controllers
{

BookController::BookController(application::IBookService* bookService) :
    m_bookService(bookService)
{
}

void BookController::setUp(QString uuid)
{
    m_bookService->setUp(QUuid(uuid));
}

mupdf::FzDocument* BookController::getFzDocument()
{
    return m_bookService->getFzDocument();
}

}  // namespace adapters::controllers