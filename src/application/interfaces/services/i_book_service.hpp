#pragma once
#include <vector>
#include <QObject>
#include "book_operation_status.hpp"
#include "book.hpp"
#include "tag.hpp"


namespace application
{

class IBookService : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IBookService() noexcept = default;
    
    virtual BookOperationStatus addBook(const QString& filePath) = 0;
    virtual BookOperationStatus deleteBook(const QString& title) = 0;
    virtual BookOperationStatus updateBook(const QString& title,
                                           const domain::models::Book& newBook) = 0;
    
    virtual const std::vector<domain::models::Book>& getBooks() const = 0;
    virtual const domain::models::Book* getBook(const QString& title) const = 0;
    virtual int getBookCount() const = 0;
    virtual BookOperationStatus setCurrentBook(const QString& title) = 0;
    virtual const domain::models::Book* getCurrentBook() const = 0;
    
    virtual BookOperationStatus addTag(const QString& title,
                                       const domain::models::Tag& tag) = 0;
    virtual BookOperationStatus removeTag(const QString& title, 
                                          const domain::models::Tag& tag) = 0;
    
signals:
    void bookCoverGenerated(int index);
    void bookInsertionStarted(int index);
    void bookInsertionEnded();
};

} // namespace application