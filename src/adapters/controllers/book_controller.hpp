#pragma once
#include <QObject>
#include <QString>
#include "i_book_controller.hpp"
#include  "i_book_service.hpp"


namespace adapters::controllers
{

class BookController : public IBookController
{
    Q_OBJECT
    
public:
    BookController(application::IBookService* bookService);
    void uploadBook(QString path) override;
    void openBook(QString path) override;
    void setCurrentBookPath(QString path) override;
    QString currentBookPath() override;
    
private:
    application::IBookService* m_bookService;
    QString m_currentBookPath;
};

} // namespace adapters::controllers