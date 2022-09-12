#pragma once
#include <QObject>
#include <QString>
#include <QList>
#include <QVariantMap>
#include "book.hpp"


namespace adapters
{

class IBookController : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IBookController() noexcept = default;
    
    
    enum OperationStatus
    {
        Success,
        BookWithTitleAlreadyExists,
        NoBookWithThisNameExists
    };
    Q_ENUM(OperationStatus)
    
    
    Q_INVOKABLE virtual void addBook(QString path) = 0;
    Q_INVOKABLE virtual void deleteBook(QString title) = 0;
    Q_INVOKABLE virtual void updateBook(QString title, QVariantMap operations) = 0;
    Q_INVOKABLE virtual void addTags(QString title, QList<QString>) = 0;
    Q_INVOKABLE virtual void removeTags(QString title, QList<QString>) = 0;
    Q_INVOKABLE virtual void books(uint amount) = 0;
    Q_INVOKABLE virtual void book(QString title) = 0;
    Q_INVOKABLE virtual bool setCurrentBook(QString title) = 0;
    Q_INVOKABLE virtual const domain::models::Book& currentBook() = 0;
};

} // namespace adapters