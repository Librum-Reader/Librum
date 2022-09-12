#pragma once
#include <QObject>
#include <QString>
#include "i_book_controller.hpp"
#include "i_book_service.hpp"


namespace adapters::controllers
{

class BookController : public IBookController
{
    Q_OBJECT
    
public:
    BookController(application::IBookService* bookService);
    
    void addBook(QString path) override;
    void deleteBook(QString title) override;
    void updateBook(QString title, QVariantMap operations) override;
    void addTags(QString title, QList<QString>) override;
    void removeTags(QString title, QList<QString>) override;
    void books(uint amount) override;
    void book(QString title) override;
    bool setCurrentBook(QString title) override;
    const domain::models::Book& currentBook() override;

private:
    application::IBookService* m_bookService;
};

} // namespace adapters::controllers