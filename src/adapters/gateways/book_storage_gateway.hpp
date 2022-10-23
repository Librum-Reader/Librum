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

    void createBook(const QString& authToken,
                    const domain::models::Book& book) override;
    void deleteBook(const QString& authToken, const QUuid& uuid) override;
    void updateBook(const QString& authToken,
                    const domain::models::Book& book) override;
    std::vector<domain::models::BookMetaData> getBooksMetaData(
            const QString& authToken) override;
    void downloadBook(const QString& authToken, const QUuid& uuid) override;
};

} // namespace adapters::gateways