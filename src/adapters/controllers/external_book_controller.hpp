#pragma once
#include <QObject>
#include <QRectF>
#include <QString>
#include "adapters_export.hpp"
#include "bookmarks_model.hpp"
#include "i_book_controller.hpp"
#include "i_book_service.hpp"
#include "search_options.hpp"
#include "toc/filtered_toc_model.hpp"

namespace adapters::controllers
{

/***
 * This class implements the IBookController interface to provide access to a
 * book that is not managed by the application, a so called 'external book'.
 *
 * This usecase occurs when e.g. using 'Open with Librum' from a file manager.
 */
class ADAPTERS_EXPORT ExternalBookController : public IBookController
{
    Q_OBJECT

public:
    ExternalBookController(application::IBookService* externalBookService);

    void setUp(QString filePath) override;
    mupdf::FzDocument* getFzDocument() override;

    void search(const QString& text) override;
    void clearSearch() override;
    void goToNextSearchHit() override;
    void goToPreviousSearchHit() override;

    const QList<domain::entities::Highlight>& getHighlights() const override;
    void addHighlight(const domain::entities::Highlight& highlight) override;
    void removeHighlight(const QUuid& uuid) override;
    void changeHighlightColor(const QUuid& uuid, const QColor& color) override;
    const domain::entities::Highlight* getHighlightAtPoint(
        const QPointF& point, int page) const override;

    const QList<domain::entities::Bookmark>& getBookmark() const override;
    QString addBookmark(const QString& name, int pageNumber,
                        float yOffset) override;
    void renameBookmark(const QString& uuid, const QString& newName) override;
    void removeBookmark(const QString& uuid) override;
    void goToBookmark(const QString& uuid) override;

    void followLink(const char* uri) override;

    QString getFilePath() const override;
    int getPageCount() const override;

    void setCurrentPage(int newCurrentPage) override;
    int getCurrentPage() const override;

    float getZoom() const override;
    void setZoom(float newZoom) override;

    bool getSearchWholeWords() const override;
    void setSearchWholeWords(bool newSearchWholeWords) override;

    bool getSearchCaseSensitive() const override;
    void setSearchCaseSensitive(bool newCaseSensitive) override;

    bool getSearchFromStart() const override;
    void setSearchFromStart(bool newSearchFromStart) override;

    QString getColorTheme() override;
    void setColorTheme(const QString& colorTheme) override;

    application::core::FilteredTOCModel* getTableOfContents() override;
    data_models::BookmarksProxyModel* getBookmarksModel() override;

private:
    application::IBookService* m_externalBookService;
    application::core::utils::SearchOptions m_searchOptions;
};

}  // namespace adapters::controllers
