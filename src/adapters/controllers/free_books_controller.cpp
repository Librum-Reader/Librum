#include "free_books_controller.hpp"

namespace adapters::controllers
{

FreeBooksController::FreeBooksController(
    application::IFreeBooksService* freeBooksService) :
    m_freeBooksService(freeBooksService)
{
    connect(m_freeBooksService,
            &application::IFreeBooksService::gettingBookFinished, this,
            &FreeBooksController::gettingBookFinished);
}

void FreeBooksController::getBooksMetadata()
{
    m_freeBooksService->getBooksMetadata();
}

void FreeBooksController::getBookMedia(const QString& url)
{
    m_freeBooksService->getBookMedia(url);
}

}  // namespace adapters::controllers
