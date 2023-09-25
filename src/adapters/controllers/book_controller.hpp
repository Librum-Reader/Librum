#pragma once
#include <QObject>
#include <QRectF>
#include <QString>
#include "adapters_export.hpp"
#include "i_book_controller.hpp"
#include "i_book_service.hpp"
#include "search_options.hpp"
#include "toc/filtered_toc_model.hpp"

namespace adapters::controllers
{

class ADAPTERS_EXPORT BookController : public IBookController
{
    Q_OBJECT

public:
    BookController(application::IBookService* bookService);

    void setUp(QString uuid) override;
    mupdf::FzDocument* getFzDocument() override;

    void search(const QString& text) override;
    void clearSearch() override;
    void goToNextSearchHit() override;
    void goToPreviousSearchHit() override;

    const QList<domain::entities::Highlight>& getHighlights() const override;
    void addHighlight(const domain::entities::Highlight& highlight) override;
    void removeHighlight(const QUuid& uuid) override;
    void changeHighlightColor(const QUuid& uuid, const QColor& color) override;
    void saveHighlights() override;
    const domain::entities::Highlight* getHighlightAtPoint(
        const QPointF& point, int page) const override;

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

    application::core::FilteredTOCModel* getTableOfContents() override;

private:
    application::IBookService* m_bookService;
    application::core::utils::SearchOptions m_searchOptions;
};

}  // namespace adapters::controllers