#pragma once
#include "i_free_books_service.hpp"
#include "i_free_books_storage_gateway.hpp"

namespace application::services
{

class FreeBooksService : public IFreeBooksService
{
    Q_OBJECT

public:
    FreeBooksService(IFreeBooksStorageGateway* freeBooksStorageGateway);

    void getBooksMetadata() override;

private:
    IFreeBooksStorageGateway* m_freeBooksStorageGateway;
};

}  // namespace application::services
