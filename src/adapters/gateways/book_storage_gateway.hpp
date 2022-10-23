#pragma once
#include <QObject>
#include "i_book_storage_gateway.hpp"

namespace adapters::gateways
{

class BookStorageGateway : public application::IBookStorageGateway
{
    Q_OBJECT
    
public:
    BookStorageGateway();
    
    void storeBook() override;
};

} // namespace adapters::gateways