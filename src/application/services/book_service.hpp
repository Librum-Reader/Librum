#pragma once
#include "i_book_service.hpp"


namespace application::services
{

class BookService : public IBookService
{
    Q_OBJECT
    
public:
    BookService();

    BookOperationStatus addBook(const QString& path) override;
    BookOperationStatus deleteBook(const QString& title) override;
    BookOperationStatus updateBook(const QString& title,
                                   const domain::models::Book& book) override;
    BookOperationStatus addTags(const QString& title, 
                                const std::vector<domain::models::Tag>& tags) override;
    BookOperationStatus removeTags(const QString& title,
                                    const std::vector<domain::models::Tag>& tags) override;
    
    void books(uint amount) const override;
    void book(const QString& title) const override;
    bool setCurrentBook(const QString& title) override;
    const domain::models::Book& currentBook() const override;
    
    
private:
    std::vector<domain::models::Book>::iterator bookWithTitleExists(const QString& title);
    std::vector<domain::models::Book>::iterator bookWithPathExists(const QString& path);
    
    domain::models::Book* m_currentBook;
    std::vector<domain::models::Book> m_books;
};

} // namespace application::services