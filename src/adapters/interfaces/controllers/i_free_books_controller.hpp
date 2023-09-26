#pragma once
#include <QObject>
#include "adapters_export.hpp"
#include "free_books_model.hpp"

namespace adapters
{

class ADAPTERS_EXPORT IFreeBooksController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(adapters::data_models::FreeBooksModel* freeBooksModel READ
                   getFreeBooksModel CONSTANT)

public:
    virtual ~IFreeBooksController() noexcept = default;

    Q_INVOKABLE virtual void fetchFirstBooksMetadataPage() = 0;
    Q_INVOKABLE virtual void getBookMedia(const int id, const QString& url) = 0;
    Q_INVOKABLE virtual void getBookCover(const int id) = 0;
    Q_INVOKABLE virtual void deleteBookCover(const int id) = 0;
    Q_INVOKABLE virtual void setFilterAuthorsAndTitle(
        const QString& authorsAndTitle) = 0;
    Q_INVOKABLE virtual QString getFilterAuthorsAndTitle() = 0;
    Q_INVOKABLE virtual void clearAllFilters() = 0;
    virtual data_models::FreeBooksModel* getFreeBooksModel() = 0;

signals:
    void startedFetchingFistMetadataPage();
    void fetchingFirstMetadataPageSuccessful(bool success);
    void foundNoBooks();
    void gettingBookFinished(const QString& path);
};

}  // namespace adapters
