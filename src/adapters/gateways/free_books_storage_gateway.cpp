#include "free_books_storage_gateway.hpp"

namespace adapters::gateways
{

FreeBooksStorageGateway::FreeBooksStorageGateway(
    IFreeBooksStorageAccess* freeBooksStorageAccess) :
    m_freeBooksStorageAccess(freeBooksStorageAccess)
{
}

void FreeBooksStorageGateway::getBooksMetadata()
{
    m_freeBooksStorageAccess->getBooksMetadata();
}

}  // namespace adapters::gateways
