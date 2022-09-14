#pragma once
#include <vector>
#include <QObject>
#include "book_operation_staus.hpp"
#include "book.hpp"
#include "tag.hpp"


namespace application
{

class IBookService : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IBookService() noexcept = default;
    
    virtual BookOperationStatus addBook(const QString& path) = 0;
    virtual BookOperationStatus deleteBook(const QString& title) = 0;
    virtual BookOperationStatus updateBook(const QString& title,
                                           const domain::models::Book& book) = 0;
    
    virtual void books(uint amount = -1) const = 0;
    virtual void book(const QString& title) const = 0;
    virtual bool setCurrentBook(const QString& title) = 0;
    virtual const domain::models::Book& currentBook() const = 0;
    
    virtual BookOperationStatus addTags(const QString& title,
                                        const std::vector<domain::models::Tag>& tags) = 0;
    virtual BookOperationStatus removeTags(const QString& title, 
                                        const std::vector<domain::models::Tag>& tags) = 0;
};

} // namespace application