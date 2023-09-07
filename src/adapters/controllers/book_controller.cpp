#include "book_controller.hpp"
#include <QUuid>

using application::core::FilteredTOCModel;

namespace adapters::controllers
{

BookController::BookController(application::IBookService* bookService) :
    m_bookService(bookService)
{
    connect(m_bookService, &application::IBookService::goToPosition, this,
            &BookController::goToPosition);
}

void BookController::setUp(QString uuid)
{
    m_bookService->setUp(QUuid(uuid));
}

mupdf::FzDocument* BookController::getFzDocument()
{
    return m_bookService->getFzDocument();
}

void BookController::followLink(const char* uri)
{
    return m_bookService->followLink(uri);
}

QString BookController::getFilePath() const
{
    return m_bookService->getFilePath();
}

int BookController::getPageCount() const
{
    return m_bookService->getPageCount();
}

void BookController::setCurrentPage(int newCurrentPage)
{
    m_bookService->setCurrentPage(newCurrentPage);
    emit currentPageChanged(newCurrentPage);
}

int BookController::getCurrentPage() const
{
    return m_bookService->getCurrentPage();
}

float BookController::getZoom() const
{
    return m_bookService->getZoom();
}

void BookController::setZoom(float newZoom)
{
    m_bookService->setZoom(newZoom);
    emit zoomChanged(newZoom);
}

FilteredTOCModel* BookController::getTableOfContents()
{
    return nullptr;
}

}  // namespace adapters::controllers