#pragma once
#include <QObject>
#include "book.hpp"
#include "tag.hpp"


namespace application
{

class IBookService : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IBookService() noexcept = default;
    
    Q_INVOKABLE virtual void addBook(const QString& path) = 0;
    Q_INVOKABLE virtual void deleteBook(const QString& title) = 0;
    Q_INVOKABLE virtual void updateBook(const QString& title, 
                                        const domain::models::Book& book) = 0;
    
    Q_INVOKABLE virtual void books(uint amount = -1) const = 0;
    Q_INVOKABLE virtual void book(const QString& title) const = 0;
    Q_INVOKABLE virtual bool setCurrentBook(const QString& title) = 0;
    Q_INVOKABLE virtual const domain::models::Book& currentBook() const = 0;
    
    Q_INVOKABLE virtual void addTags(const QString& title, 
                                     const QList<domain::models::Tag>& tags) = 0;
    Q_INVOKABLE virtual void removeTags(const QString& title, 
                                        const QList<domain::models::Tag>& tags) = 0;
    
};

} // namespace application