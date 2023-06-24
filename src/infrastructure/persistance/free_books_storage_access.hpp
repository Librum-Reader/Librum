#pragma once
#include "i_free_books_storage_access.hpp"

namespace infrastructure::persistence
{

class FreeBooksStorageAccess : public adapters::IFreeBooksStorageAccess
{
    Q_OBJECT
};

}  // namespace infrastructure::persistence
