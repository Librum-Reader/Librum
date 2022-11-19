#pragma once
#include "i_book_storage_manager.hpp"

namespace application::utility
{

class BookStorageManager : public IBookStorageManager
{
public:
    void addBook(const domain::models::Book& bookToAdd) override;
    void deleteBook(const QUuid& uuid) override;
    void uninstallBook(const QUuid& uuid) override;
    void updateBook(const domain::models::Book& newBook) override;
    std::vector<domain::models::Book> loadBooks() override;
};

}  // namespace application::utility