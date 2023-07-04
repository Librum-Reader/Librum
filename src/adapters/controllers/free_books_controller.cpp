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

void FreeBooksController::getBooksMetadata(const QString& author,
                                           const QString& title)
{
    m_freeBooksService->getBooksMetadata(author, title);
}

void FreeBooksController::getBookMedia(const QString& url)
{
    m_freeBooksService->getBookMedia(url);
}

}  // namespace adapters::controllers
