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
    
    int addBook(const QString& path) override;
    int deleteBook(const QString& title) override;
    int updateBook(const QString& title,
                   const QVariantMap& operations) override;
    int addTag(const QString& title,
               const dtos::TagDto& tag) override;
    int removeTag(const QString& title,
                  const QString& tagName) override;
    const dtos::BookDto* getBook(const QString& title) override;
    int getBookCount() const override;
    int setCurrentBook(QString title) override;
    dtos::BookDto getCurrentBook() override;
    
    data_models::LibraryModel* getLibraryModel() override;
    
private:
    void refreshBookChache();
    dtos::BookDto* getBookFromChache(const QString& title);
    bool refreshCurrentBookChache();
    
    bool m_bookChacheChanged;
    std::vector<dtos::BookDto> m_bookCache;
    bool m_currentBookCacheChanged;
    dtos::BookDto m_currentBookCache;
    
    application::IBookService* m_bookService;
    data_models::LibraryModel m_libraryModel;
};

} // namespace adapters::controllers