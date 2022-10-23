#pragma once
#include <vector>
#include <QObject>
#include <QString>
#include "book_dto.hpp"


namespace adapters
{

class IBookStorageAccess : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IBookStorageAccess() noexcept = default;
    
    virtual void createBook(const QString& authToken,
                            const dtos::BookDto& bookDto) = 0;
    virtual void deleteBook(const QString& authToken,
                            const QUuid& uuid) = 0;
    virtual void updateBook(const QString& authToken,
                            const dtos::BookDto& bookDto) = 0;
    virtual std::vector<dtos::BookDto> getBooksMetaData(const QString& 
                                                        authToken) = 0;
    virtual void downloadBook(const QString& authToken,
                              const QUuid& uuid) = 0;
    
signals:
    void creatingBookFinished(bool success, const QString& reason);
    void deletingBookFinished(bool success, const QString& reason);
    void updatingBookFinished(bool success, const QString& reason);
    void gettingBooksMetaDataFinished(const std::vector<dtos::BookDto>& metadata);
    void downloadingBookFinisdhed(const QUuid& uuid, const QByteArray& data);
};

} // namespace adapters