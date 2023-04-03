#pragma once
#include <QString>
#include "i_book_storage_gateway.hpp"
#include "i_book_storage_manager.hpp"
#include "i_downloaded_books_tracker.hpp"

namespace application::utility
{

class BookStorageManager : public IBookStorageManager
{
public:
    BookStorageManager(IBookStorageGateway* bookStorageGateway,
                       IDownloadedBooksTracker* downloadedBooksTracker);

    void addBook(const domain::entities::Book& bookToAdd) override;
    void deleteBook(BookForDeletion bookToDelete) override;
    void uninstallBook(const QUuid& uuid) override;
    std::optional<QUrl> downloadBook(const QUuid& uuid) override;
    void updateBook(const domain::entities::Book& book) override;
    void updateBookLocally(const domain::entities::Book& book) override;
    void updateBookRemotely(const domain::entities::Book& book) override;
    std::vector<domain::entities::Book> loadLocalBooks() override;
    void loadRemoteBooks() override;

    void setUserData(const QString& email, const QString& authToken) override;
    void clearUserData() override;

private:
    bool userLoggedIn();

    IBookStorageGateway* m_bookStorageGateway;
    IDownloadedBooksTracker* m_downloadedBooksTracker;
    QString m_authenticationToken;
};

}  // namespace application::utility