#pragma once
#include <memory>
#include "book.hpp"
#include "i_book_service.hpp"
#include "i_library_service.hpp"
#include "mupdf/classes.h"

namespace application::services
{

class BookService : public IBookService
{
public:
    BookService(ILibraryService* libraryService);

    void setUp(QUuid uuid) override;
    mupdf::FzDocument* getFzDocument() override;

private:
    domain::entities::Book* m_book = nullptr;
    ILibraryService* m_libraryService;
    std::unique_ptr<mupdf::FzDocument> m_fzDocument = nullptr;
};

}  // namespace application::services