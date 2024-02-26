#pragma once
#include <memory>
#include "i_book_getter.hpp"

namespace application::utility
{

/*
 * This class is a basic implementation of IBookGetter. It holds a
 * representation of an 'external book'. An external book is a book that is not
 * in the user's library.
 */
class ExternalBookGetter : public IBookGetter
{
public:
    ExternalBookGetter(const QString& filePath);

    domain::entities::Book* getBook() override;
    void setUuid(const QUuid& uuid) override;
    void updateBook(domain::entities::Book* book) override;

private:
    std::unique_ptr<domain::entities::Book> m_externalBook;
};

}  // namespace application::utility