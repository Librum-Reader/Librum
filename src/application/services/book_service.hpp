#pragma once
#include <QPixmap>
#include "i_book_info_helper.hpp"
#include "i_book_service.hpp"


namespace application::services
{

class BookService : public IBookService
{
    Q_OBJECT
    
public:
    BookService(IBookInfoHelper* bookInfoManager);

    BookOperationStatus addBook(const QString& filePath) override;
    BookOperationStatus deleteBook(const QString& title) override;
    BookOperationStatus updateBook(const QString& title,
                                   const domain::models::Book& newBook) override;
    
    BookOperationStatus addTag(const QString& title, 
                               const domain::models::Tag& tag) override;
    BookOperationStatus removeTag(const QString& title,
                                  const domain::models::Tag& tag) override;
        
    const std::vector<domain::models::Book>& getBooks() const override;
    const domain::models::Book* getBook(const QString& title) const override;
    int getBookIndex(const QString& title) const override;
    int getBookCount() const override;
    
public slots:
    bool refreshLastOpenedFlag(const QString& title) override;
    
private slots:
    void storeBookCover(const QPixmap* pixmap);
    
private:
    domain::models::Book* getBookByTitle(const QString& title);
    const domain::models::Book* getBookByTitle(const QString& title) const;
    QString getCurrentDateTimeAsString();
    
    IBookInfoHelper* m_bookInfoManager;
    std::vector<domain::models::Book> m_books;
};

} // namespace application::services