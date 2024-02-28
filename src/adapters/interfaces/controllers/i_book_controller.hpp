#include <QObject>
#include "adapters_export.hpp"
#include "bookmark.hpp"
#include "bookmarks_proxy_model.hpp"
#include "highlight.hpp"
#include "mupdf/classes.h"
#include "toc/filtered_toc_model.hpp"
#pragma once

namespace adapters
{

/**
 * The BookController class is set up to work one one book at the time.
 * It has access to the IBookService interface, which makes it possible to get
 * information about a specific book such as getting its highlights, the
 * underyling FzDocument and similar.
 */
class ADAPTERS_EXPORT IBookController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filePath NOTIFY filePathChanged)
    Q_PROPERTY(int pageCount READ getPageCount NOTIFY pageCountChanged)
    Q_PROPERTY(int currentPage READ getCurrentPage WRITE setCurrentPage NOTIFY
                   currentPageChanged)
    Q_PROPERTY(float zoom READ getZoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(application::core::FilteredTOCModel* tableOfContents READ
                   getTableOfContents NOTIFY tableOfContentsChanged)
    Q_PROPERTY(adapters::data_models::BookmarksProxyModel* bookmarksModel READ
                   getBookmarksModel NOTIFY bookmarksModelChanged)
    Q_PROPERTY(bool searchWholeWords READ getSearchWholeWords WRITE
                   setSearchWholeWords NOTIFY searchWholeWordsChanged)
    Q_PROPERTY(bool searchCaseSensitive READ getSearchCaseSensitive WRITE
                   setSearchCaseSensitive NOTIFY searchCaseSensitiveChanged)
    Q_PROPERTY(bool searchFromStart READ getSearchFromStart WRITE
                   setSearchFromStart NOTIFY searchFromStartChanged)
    Q_PROPERTY(QString colorTheme READ getColorTheme WRITE setColorTheme NOTIFY
                   colorThemeChanged)

public:
    virtual ~IBookController() noexcept = default;

    Q_INVOKABLE virtual bool setUp(QString filePath) = 0;
    virtual mupdf::FzDocument* getFzDocument() = 0;

    Q_INVOKABLE virtual void search(const QString& text) = 0;
    Q_INVOKABLE virtual void clearSearch() = 0;
    Q_INVOKABLE virtual void goToNextSearchHit() = 0;
    Q_INVOKABLE virtual void goToPreviousSearchHit() = 0;

    virtual const QList<domain::entities::Highlight>& getHighlights() const = 0;
    virtual void addHighlight(const domain::entities::Highlight& highlight) = 0;
    virtual void removeHighlight(const QUuid& uuid) = 0;
    virtual void changeHighlightColor(const QUuid& uuid,
                                      const QColor& color) = 0;
    virtual const domain::entities::Highlight* getHighlightAtPoint(
        const QPointF& point, int page) const = 0;

    virtual const QList<domain::entities::Bookmark>& getBookmark() const = 0;
    Q_INVOKABLE virtual QString addBookmark(const QString& name, int pageNumber,
                                            float yOffset) = 0;
    Q_INVOKABLE virtual void renameBookmark(const QString& uuid,
                                            const QString& newName) = 0;
    Q_INVOKABLE virtual void removeBookmark(const QString& uuid) = 0;
    Q_INVOKABLE virtual void goToBookmark(const QString& uuid) = 0;

    virtual void followLink(const char* uri) = 0;

    virtual QString getFilePath() const = 0;
    virtual void setCurrentPage(int newCurrentPage) = 0;

    virtual int getPageCount() const = 0;
    virtual int getCurrentPage() const = 0;

    virtual float getZoom() const = 0;
    virtual void setZoom(float newZoom) = 0;

    virtual bool getSearchWholeWords() const = 0;
    virtual void setSearchWholeWords(bool newSearchWholeWords) = 0;

    virtual bool getSearchCaseSensitive() const = 0;
    virtual void setSearchCaseSensitive(bool newSearchCaseSensitive) = 0;

    virtual bool getSearchFromStart() const = 0;
    virtual void setSearchFromStart(bool newSearchFromStart) = 0;

    virtual QString getColorTheme() = 0;
    virtual void setColorTheme(const QString& colorTheme) = 0;

    virtual application::core::FilteredTOCModel* getTableOfContents() = 0;
    virtual adapters::data_models::BookmarksProxyModel* getBookmarksModel() = 0;

signals:
    void filePathChanged(const QString& filePath);
    void pageCountChanged(int pageCount);
    void currentPageChanged(int currentPage);
    void zoomChanged(float zoom);
    void tableOfContentsChanged();
    void goToPosition(int pageNumber, int y);
    void selectText(int pageNumber, QPointF left, QPointF right);
    void textSelectionFinished(float centerX, float topY);
    void highlightSelected(float centerX, float topY, const QString& uuid);
    void noSearchHitsFound();
    void searchWholeWordsChanged();
    void searchCaseSensitiveChanged();
    void searchFromStartChanged();
    void bookmarksModelChanged();
    void colorThemeChanged(const QString& colorTheme);
};

}  // namespace adapters
