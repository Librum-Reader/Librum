#include "book_service.hpp"

namespace application::services
{

BookService::BookService(IPdfToRumConverter* pdfToRumConverter)
    : m_pdfToRumConverter(pdfToRumConverter)
{
    
}

void BookService::uploadBook(QString path)
{
    m_pdfToRumConverter->convertToRum(path);
}

} // namespace application::services