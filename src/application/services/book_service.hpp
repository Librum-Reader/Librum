#pragma once
#include <QObject>
#include "i_book_service.hpp"
#include "i_pdf_to_rum_converter.hpp"


namespace application::services
{

class BookService : public IBookService
{
    Q_OBJECT
    
public:
    BookService(IPdfToRumConverter* pdfToRumConverter);

    void uploadBook(QString path) override;
    
    
private:
    IPdfToRumConverter* m_pdfToRumConverter;
};

} // namespace application::services