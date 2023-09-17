#pragma once
#include <QColor>
#include <QDir>
#include <QObject>
#include <QString>
#include <QUuid>
#include "application_export.hpp"
#include "highlight.hpp"
#include "mupdf/classes.h"
#include "toc/filtered_toc_model.hpp"

namespace application
{

/**
 * The BookService is set up with one specific book and operates only on it. It
 * is able to get and change information on one specific book.
 */
class APPLICATION_EXPORT IBookService : public QObject
{
    Q_OBJECT

public:
    virtual ~IBookService() noexcept = default;

    virtual void setUp(QUuid uuid) = 0;
    virtual mupdf::FzDocument* getFzDocument() = 0;

    virtual void search(const QString& text) = 0;
    virtual void clearSearch() = 0;
    virtual void goToNextSearchHit() = 0;
    virtual void goToPreviousSearchHit() = 0;

    virtual const QList<domain::entities::Highlight>& getHighlights() const = 0;
    virtual void addHighlight(const domain::entities::Highlight& highlight) = 0;
    virtual void removeHighlight(const QUuid& uuid) = 0;
    virtual void changeHighlightColor(const QUuid& uuid,
                                      const QColor& color) = 0;
    virtual void saveHighlights() = 0;
    virtual const domain::entities::Highlight* getHighlightAtPoint(
        const QPointF& point, int page) const = 0;

    virtual void followLink(const char* uri) = 0;

    virtual QString getFilePath() const = 0;
    virtual int getPageCount() const = 0;
    virtual void setCurrentPage(int newCurrentPage) = 0;
    virtual int getCurrentPage() const = 0;
    virtual float getZoom() const = 0;
    virtual void setZoom(float newZoom) = 0;

    virtual core::FilteredTOCModel* getTableOfContents() = 0;

signals:
    void goToPosition(int pageNumber, int y);
    void highlightText(int pageNumber, mupdf::FzQuad quad);
    void noSearchHitsFound();
};

}  // namespace application