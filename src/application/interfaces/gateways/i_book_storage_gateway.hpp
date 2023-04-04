#pragma once
#include <QObject>
#include <QString>
#include <QUuid>
#include <vector>
#include "book.hpp"

namespace application
{

/**
 * The BookStorageGateway class acts as a layer of abstraction before the
 * BookStorageAccess class. It maps the data provided by the application to
 * the data type required for book storage API requests.
 */
class IBookStorageGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~IBookStorageGateway() noexcept = default;

    virtual void createBook(const QString& authToken,
                            const domain::entities::Book& book) = 0;
    virtual void deleteBook(const QString& authToken, const QUuid& uuid) = 0;
    virtual void updateBook(const QString& authToken,
                            const domain::entities::Book& book) = 0;
    virtual void getBooksMetaData(const QString& authToken) = 0;
    virtual void downloadBook(const QString& authToken, const QUuid& uuid) = 0;

signals:
    void creatingBookFinished(bool success, const QString& reason);
    void deletingBookFinished(bool success, const QString& reason);
    void updatingBookFinished(bool success, const QString& reason);
    void gettingBooksMetaDataFinished(
        const std::vector<domain::entities::Book>& books);
    void downloadingBookFinished(const QByteArray& data, const QUuid& uuid);
};

}  // namespace application