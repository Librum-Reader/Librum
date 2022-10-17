#pragma once
#include <QPixmap>
#include "i_book_metadata_helper.hpp"
#include "i_book_service.hpp"
#include "i_downloaded_books_tracker.hpp"


namespace application::services
{

class BookService : public IBookService
{
    Q_OBJECT
    
public:
    BookService(IBookMetadataHelper* bookMetadataHelper, 
                IDownloadedBooksTracker* downloadedBooksTracker);

    BookOperationStatus addBook(const QString& filePath) override;
    BookOperationStatus deleteBook(const QUuid& uuid) override;
    BookOperationStatus updateBook(const QUuid& uuid,
                                   const domain::models::Book& newBook) override;
    
    BookOperationStatus addTag(const QUuid& uuid, 
                               const domain::models::Tag& tag) override;
    BookOperationStatus removeTag(const QUuid& uuid,
                                  const domain::models::Tag& tag) override;
        
    const std::vector<domain::models::Book>& getBooks() const override;
    const domain::models::Book* getBook(const QUuid& uuid) const override;
    domain::models::Book* getBook(const QUuid& uuid) override;
    int getBookIndex(const QUuid& uuid) const override;
    int getBookCount() const override;
    
    BookOperationStatus saveBookToPath(const QUuid& uuid,
                                       const QUrl& path) override;
    
public slots:
    bool refreshLastOpenedFlag(const QUuid& uuid) override;
    void setAuthenticationToken(const QString& token,
                                const QString& email) override;
    void clearAuthenticationToken() override;
    
private slots:
    void storeBookCover(const QPixmap* pixmap);
    
private:
    QString getCurrentDateTimeAsString();
    void loadLocalBooks();
    
    IBookMetadataHelper* m_bookMetadataHelper;
    IDownloadedBooksTracker* m_downloadedBooksTracker;
    std::vector<domain::models::Book> m_books;
    QString m_authenticationToken;
    QString m_currentUserEmail;
};

} // namespace application::services