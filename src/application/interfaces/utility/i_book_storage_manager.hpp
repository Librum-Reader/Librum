#pragma once
#include <QObject>
#include <QUuid>
#include <vector>
#include "book.hpp"

namespace application
{

/*
 * Manages the book storage for the local and remote library.
 */
class IBookStorageManager : public QObject
{
    Q_OBJECT

public:
    virtual ~IBookStorageManager() noexcept = default;

    virtual void addBook(const domain::models::Book& bookToAdd) = 0;
    virtual void deleteBook(const QUuid& uuid) = 0;
    virtual void uninstallBook(const QUuid& uuid) = 0;
    virtual void updateBook(const domain::models::Book& newBook) = 0;
    virtual std::vector<domain::models::Book> loadBooks() = 0;
};

}  // namespace application