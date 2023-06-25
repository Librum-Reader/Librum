#pragma once
#include "i_free_books_storage_access.hpp"
#include "i_free_books_storage_gateway.hpp"

namespace adapters::gateways
{

class FreeBooksStorageGateway : public application::IFreeBooksStorageGateway
{
    Q_OBJECT

public:
    FreeBooksStorageGateway(IFreeBooksStorageAccess* freeBooksStorageAccess);

    void getBooksMetadata() override;

private:
    IFreeBooksStorageAccess* m_freeBooksStorageAccess;
};

}  // namespace adapters::gateways
