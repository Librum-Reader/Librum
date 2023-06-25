#include "free_books_service.hpp"

namespace application::services
{

FreeBooksService::FreeBooksService(
    IFreeBooksStorageGateway* freeBooksStorageGateway) :
    m_freeBooksStorageGateway(freeBooksStorageGateway)
{
}

void FreeBooksService::getBooksMetadata()
{
    m_freeBooksStorageGateway->getBooksMetadata();
}

}  // namespace application::services
