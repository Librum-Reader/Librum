#pragma once
#include <vector>
#include <QObject>
#include <QString>
#include <QString>
#include <QUuid>
#include "book.hpp"


namespace application
{

class IBookStorageGateway : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IBookStorageGateway() noexcept = default;
    
    virtual void createBook(const QString& authToken,
                            const domain::models::Book& book) = 0;
    virtual void deleteBook(const QString& authToken,
                            const QUuid& uuid) = 0;
    virtual void updateBook(const QString& authToken,
                            const domain::models::Book& book) = 0;
    virtual void getBooksMetaData(const QString& authToken) = 0;
    virtual void downloadBook(const QString& authToken,
                              const QUuid& uuid) = 0;
    
signals:
    void creatingBookFinished(bool success, const QString& reason);
    void deletingBookFinished(bool success, const QString& reason);
    void updatingBookFinished(bool success, const QString& reason);
    void gettingBooksMetaDataFinished(const std::vector<domain::models::
                                      Book>& books);
    void downloadingBookFinisdhed(const QUuid& uuid, const QByteArray& data);
};

} // namespace application#pragma once