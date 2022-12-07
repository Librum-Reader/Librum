#pragma once
#include <QObject>
#include <QString>
#include <QUuid>
#include <vector>
#include "book.hpp"
#include "tag.hpp"

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

    virtual void addBook(const domain::models::Book& bookToAdd) = 0;
    virtual void deleteBook(const QUuid& uuid) = 0;
    virtual void uninstallBook(const QUuid& uuid) = 0;
    virtual void updateBook(const domain::models::Book& book) = 0;
    virtual void updateBookLocally(const domain::models::Book& book) = 0;
    virtual void updateBookRemotely(const domain::models::Book& book) = 0;
    virtual std::vector<domain::models::Book> loadLocalBooks() = 0;
    virtual void loadRemoteBooks() = 0;
    virtual void addTag(const QUuid& bookUuid,
                        const domain::models::Tag& tag) = 0;
    virtual void deleteTag(const QUuid& bookUuid, const QUuid& tagUuid) = 0;

    virtual void setUserData(const QString& email,
                             const QString& authToken) = 0;
    virtual void clearUserData() = 0;

signals:
    void loadingRemoteBooksFinished(
        const std::vector<domain::models::Book>& books);
};

}  // namespace application