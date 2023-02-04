#pragma once
#include <QObject>
#include <QString>
#include <QUuid>
#include <vector>
#include "book.hpp"
#include "book_for_deletion.hpp"

namespace application
{

/**
 * The BookStorageManager manages the book storage for the local and remote
 * library.
 */
class IBookStorageManager : public QObject
{
    Q_OBJECT

public:
    virtual ~IBookStorageManager() noexcept = default;

    virtual void addBook(const domain::entities::Book& bookToAdd) = 0;
    virtual void deleteBook(utility::BookForDeletion bookToDelete) = 0;
    virtual void uninstallBook(const QUuid& uuid) = 0;
    virtual void updateBook(const domain::entities::Book& book) = 0;
    virtual void updateBookLocally(const domain::entities::Book& book) = 0;
    virtual void updateBookRemotely(const domain::entities::Book& book) = 0;
    virtual std::vector<domain::entities::Book> loadLocalBooks() = 0;
    virtual void loadRemoteBooks() = 0;

    virtual void setUserData(const QString& email,
                             const QString& authToken) = 0;
    virtual void clearUserData() = 0;

signals:
    void loadingRemoteBooksFinished(
        const std::vector<domain::entities::Book>& books);
};

}  // namespace application