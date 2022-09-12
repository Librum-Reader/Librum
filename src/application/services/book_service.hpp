#pragma once
#include <QObject>
#include <QList>
#include "i_book_service.hpp"


namespace application::services
{

class BookService : public IBookService
{
    Q_OBJECT
    
public:
    BookService();

    void addBook(const QString& path) override;
    void deleteBook(const QString& title) override;
    void updateBook(const QString& title, const domain::models::Book& book) override;
    void addTags(const QString& title, const QList<domain::models::Tag>& tags) override;
    void removeTags(const QString& title, 
                    const QList<domain::models::Tag>& tags) override;
    void books(uint amount) const override;
    void book(const QString& title) const override;
    bool setCurrentBook(const QString& title) override;
    const domain::models::Book& currentBook() const override;
    
private:
    domain::models::Book* m_currentBook;
    QList<const domain::models::Book*> m_books;
};

} // namespace application::services