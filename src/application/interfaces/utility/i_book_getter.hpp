#pragma once
#include <QUuid>
#include "application_export.hpp"
#include "book.hpp"

namespace application
{

/**
 * This is a basic interface to get a pointer to a book from any source.
 */
class APPLICATION_EXPORT IBookGetter
{
public:
    virtual ~IBookGetter() noexcept = default;

    virtual domain::entities::Book* getBook() = 0;
    virtual void setUuid(const QUuid& uuid) = 0;
    virtual void updateBook(domain::entities::Book* book) = 0;
    virtual bool bookIsValid() = 0;
};

}  // namespace application