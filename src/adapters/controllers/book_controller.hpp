#pragma once
#include <QObject>
#include <QString>
#include "book_dto.hpp"
#include "i_book_controller.hpp"
#include "i_book_service.hpp"


namespace adapters::controllers
{

class BookController : public IBookController
{
    Q_OBJECT
    
public:
    BookController(application::IBookService* bookService);
    
    application::BookOperationStatus addBook(const QString& path) override;
    application::BookOperationStatus deleteBook(const QString& title) override;
    application::BookOperationStatus updateBook(const QString& title,
                                                const QVariantMap& operations) override;
    application::BookOperationStatus addTag(const QString& title,
                                              const dtos::TagDto& tag) override;
    application::BookOperationStatus removeTag(const QString& title,
                                               const QString& tagName) override;
    const dtos::BookDto* getBook(const QString& title) override;
    int getBookCount() const override;
    application::BookOperationStatus setCurrentBook(QString title) override;
    const dtos::BookDto* getCurrentBook() override;
    
private:
    void refreshBookChache();
    dtos::BookDto* getBookFromChache(const QString& title);
    bool refreshCurrentBookChache();
    
    application::IBookService* m_bookService;
    bool m_bookChacheChanged;
    std::vector<dtos::BookDto> m_bookCache;
    bool m_currentBookCacheChanged;
    dtos::BookDto m_currentBookCache;
};

} // namespace adapters::controllers