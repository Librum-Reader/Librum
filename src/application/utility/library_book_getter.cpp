#include "library_book_getter.hpp"
#include <QDebug>

using domain::entities::Book;

namespace application::utility
{

LibraryBookGetter::LibraryBookGetter(ILibraryService* libraryService,
                                     const QUuid& uuid) :
    m_libraryService(libraryService),
    m_uuid(uuid)
{
}

Book* application::utility::LibraryBookGetter::getBook()
{
    auto* book = m_libraryService->getBook(m_uuid);
    if(book == nullptr)
        qWarning() << "Failed getting book with uuid: " << m_uuid;

    return book;
}

void LibraryBookGetter::setUuid(const QUuid& uuid)
{
    m_uuid = uuid;
}

void LibraryBookGetter::updateBook(Book* book)
{
    m_libraryService->updateBook(*book);
}

bool LibraryBookGetter::bookIsValid()
{
    return true;
}

}  // namespace application::utility