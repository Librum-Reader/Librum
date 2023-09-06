#include "book_service.hpp"
#include <QDebug>
#include <string>

namespace application::services
{

BookService::BookService(ILibraryService* libraryService) :
    m_libraryService(libraryService)
{
}

void BookService::setUp(QUuid uuid)
{
    m_book = m_libraryService->getBook(uuid);
    if(m_book == nullptr)
    {
        qDebug() << "Failed opening book with uuid: " << uuid;
        return;
    }

    auto stdFilePath = m_book->getFilePath().toStdString();
    m_fzDocument = std::make_unique<mupdf::FzDocument>(stdFilePath.c_str());
}

mupdf::FzDocument* BookService::getFzDocument()
{
    return m_fzDocument.get();
}

}  // namespace application::services