#pragma once
#include "free_books_model.hpp"
#include "i_free_books_controller.hpp"
#include "i_free_books_service.hpp"

namespace adapters::controllers
{

class FreeBooksController : public IFreeBooksController
{
    Q_OBJECT

public:
    FreeBooksController(application::IFreeBooksService* bookService);

    void getBooksMetadata(const QString& author, const QString& title) override;
    void getBookMedia(const int id, const QString& url) override;
    void getBookCover(const int id) override;
    void deleteBookCover(const int id) override;
    data_models::FreeBooksModel* getFreeBooksModel() override;

private:
    application::IFreeBooksService* m_freeBooksService;
    data_models::FreeBooksModel m_freeBooksModel;
};

}  // namespace adapters::controllers
