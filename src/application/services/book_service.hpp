#pragma once
#include <QObject>
#include "i_book_service.hpp"


namespace application::services
{

class BookService : public IBookService
{
    Q_OBJECT
    
public:
    BookService();

    void uploadBook(QString path) override;
    void openBook(QString path) override;
};

} // namespace application::services