#pragma once
#include "adapters_export.hpp"
#include "free_books_model.hpp"
#include "i_free_books_controller.hpp"
#include "i_free_books_service.hpp"

namespace adapters::controllers
{

class ADAPTERS_EXPORT FreeBooksController : public IFreeBooksController
{
    Q_OBJECT

public:
    FreeBooksController(application::IFreeBooksService* bookService);

    void fetchFirstBooksMetadataPage() override;
    void getBookMedia(const int id, const QString& url) override;
    void getBookCover(const int id) override;
    void deleteBookCover(const int id) override;
    void setFilterAuthorsAndTitle(const QString& authorsAndTitle) override;
    QString getFilterAuthorsAndTitle() override;
    void clearAllFilters() override;
    bool isBookDownloaded(int id) override;
    void proccessDownloadedProjectGutenbergBookIds(
        const std::set<int>& ids) override;
    void markBookAsDownloaded(int id) override;
    void unmarkBookAsDownloaded(int id) override;
    data_models::FreeBooksModel* getFreeBooksModel() override;

private slots:
    void proccessFetchingFirstMetadataPageResult(bool result);

private:
    application::IFreeBooksService* m_freeBooksService;
    data_models::FreeBooksModel m_freeBooksModel;
    QString m_filterAuthorsAndTitle;
    bool m_firstPageIsLoaded = false;

    void deleteAllBooks();
};

}  // namespace adapters::controllers
