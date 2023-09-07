#include <QObject>
#include "adapters_export.hpp"
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

public:
    virtual ~IBookController() noexcept = default;

    Q_INVOKABLE virtual void setUp(QString uuid) = 0;
    virtual mupdf::FzDocument* getFzDocument() = 0;

    Q_INVOKABLE virtual void search(const QString& text) = 0;
    Q_INVOKABLE virtual void clearSearch() = 0;
    Q_INVOKABLE virtual void goToNextSearchHit() = 0;
    Q_INVOKABLE virtual void goToPreviousSearchHit() = 0;

    virtual void followLink(const char* uri) = 0;

    virtual QString getFilePath() const = 0;
    virtual int getPageCount() const = 0;
    virtual int getCurrentPage() const = 0;
    virtual void setCurrentPage(int newCurrentPage) = 0;
    virtual float getZoom() const = 0;
    virtual void setZoom(float newZoom) = 0;
    virtual application::core::FilteredTOCModel* getTableOfContents() = 0;

signals:
    void filePathChanged(const QString& filePath);
    void pageCountChanged(int pageCount);
    void currentPageChanged(int currentPage);
    void zoomChanged(float zoom);
    void tableOfContentsChanged();
    void goToPosition(int pageNumber, int y);
    void highlightText(int pageNumber, QPointF left, QPointF right);
};

}  // namespace adapters