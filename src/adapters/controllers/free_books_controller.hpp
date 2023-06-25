#pragma once
#include "i_free_books_controller.hpp"
#include "i_free_books_service.hpp"

namespace adapters::controllers
{

class FreeBooksController : public IFreeBooksController
{
    Q_OBJECT

public:
    FreeBooksController(application::IFreeBooksService* bookService);

    void getBooksMetadata() override;

private:
    application::IFreeBooksService* m_freeBooksService;
};

}  // namespace adapters::controllers
