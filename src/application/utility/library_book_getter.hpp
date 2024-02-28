#pragma once
#include "i_book_getter.hpp"
#include "i_library_service.hpp"

namespace application::utility
{

/*
 * This class is a basic implementation of IBookGetter. It gets a book from a
 * library service.
 */
class LibraryBookGetter : public IBookGetter
{
public:
    LibraryBookGetter(ILibraryService* libraryService, const QUuid& uuid);

    domain::entities::Book* getBook() override;
    void setUuid(const QUuid& uuid) override;
    void updateBook(domain::entities::Book* book) override;
    bool bookIsValid() override;

private:
    ILibraryService* m_libraryService;
    QUuid m_uuid;
};

}  // namespace application::utility