#pragma once
#include <QObject>
#include <QString>
#include <QList>
#include <QVariantMap>
#include "book_dto.hpp"
#include "book_operation_status.hpp"


namespace adapters
{

class IBookController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const dtos::BookDto* currentBook READ getCurrentBook NOTIFY currentBookChanged)
    Q_PROPERTY(int bookCount READ getBookCount NOTIFY bookCountChanged)
    
public:
    virtual ~IBookController() noexcept = default;
    
    
    Q_INVOKABLE virtual application::BookOperationStatus addBook(const QString& path) = 0;
    Q_INVOKABLE virtual application::BookOperationStatus deleteBook(const QString& title) = 0;
    Q_INVOKABLE virtual application::BookOperationStatus updateBook(const QString& title,
                                                                    const QVariantMap& operations) = 0;
    Q_INVOKABLE virtual application::BookOperationStatus addTag(const QString& title,
                                                                const dtos::TagDto& tag) = 0;
    Q_INVOKABLE virtual application::BookOperationStatus removeTag(const QString& title,
                                                                   const QString& tagName) = 0;
    Q_INVOKABLE virtual const dtos::BookDto* getBook(const QString& title) = 0;
    Q_INVOKABLE virtual int getBookCount() const = 0;
    Q_INVOKABLE virtual application::BookOperationStatus setCurrentBook(QString title) = 0;
    Q_INVOKABLE virtual const dtos::BookDto* getCurrentBook() = 0;
    
signals:
    void currentBookChanged();
    void bookCountChanged();
};

} // namespace adapters