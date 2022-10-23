#pragma once
#include "i_book_storage_access.hpp"


namespace infrastructure::persistence
{

class BookStorageAccess : public adapters::IBookStorageAccess
{
    Q_OBJECT
    
public:
    BookStorageAccess();
    
    void storeBook() override;
};

} // namespace infrastructure::persistence