#include "free_books_controller.hpp"

namespace adapters::controllers
{

FreeBooksController::FreeBooksController(
    application::IFreeBooksService* freeBooksService) :
    m_freeBooksService(freeBooksService)
{
}

void FreeBooksController::getBooksMetadata()
{
    m_freeBooksService->getBooksMetadata();
}

}  // namespace adapters::controllers
