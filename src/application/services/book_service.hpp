#pragma once
#include "i_book_info_manager.hpp"
#include "i_book_service.hpp"


namespace application::services
{

class BookService : public IBookService
{
    Q_OBJECT
    
public:
    BookService(IBookInfoManager* bookInfoManager);

    BookOperationStatus addBook(const QString& filePath) override;
    BookOperationStatus deleteBook(const QString& title) override;
    BookOperationStatus updateBook(const QString& title,
                                   const domain::models::Book& book) override;
    BookOperationStatus addTag(const QString& title, 
                               const domain::models::Tag& tag) override;
    BookOperationStatus removeTag(const QString& title,
                                  const domain::models::Tag& tag) override;
    
    const std::vector<domain::models::Book>& getAllBooks() const override;
    const domain::models::Book* getBook(const QString& title) const override;
    int getBookCount() const override;
    bool setCurrentBook(const QString& title) override;
    const domain::models::Book* getCurrentBook() const override;
    
    
private:
    std::vector<domain::models::Book>::iterator getBookByTitle(const QString& title);
    const std::vector<domain::models::Book>::const_iterator getBookByTitle(const QString& title) const;
    std::vector<domain::models::Book>::iterator getBookByPath(const QString& path);
    
    domain::models::Book* m_currentBook;
    std::vector<domain::models::Book> m_books;
    IBookInfoManager* m_bookInfoManager;
};

} // namespace application::services