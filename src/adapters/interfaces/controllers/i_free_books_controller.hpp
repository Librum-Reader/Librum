#pragma once
#include <QObject>
#include "free_books_model.hpp"

namespace adapters
{

class IFreeBooksController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(adapters::data_models::FreeBooksModel* freeBooksModel READ
                   getFreeBooksModel CONSTANT)

public:
    virtual ~IFreeBooksController() noexcept = default;

    Q_INVOKABLE virtual void fetchFirstBooksMetadataPageWithFilter(
        const QString& author, const QString& title) = 0;
    Q_INVOKABLE virtual void getBookMedia(const int id, const QString& url) = 0;
    Q_INVOKABLE virtual void getBookCover(const int id) = 0;
    Q_INVOKABLE virtual void deleteBookCover(const int id) = 0;
    virtual data_models::FreeBooksModel* getFreeBooksModel() = 0;

signals:
    void gettingBookFinished(const QString& path);
};

}  // namespace adapters
