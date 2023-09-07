#pragma once
#include <QObject>
#include <QRectF>
#include <QString>
#include "adapters_export.hpp"
#include "i_book_controller.hpp"
#include "i_book_service.hpp"
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

    void followLink(const char* uri) override;

    QString getFilePath() const override;
    int getPageCount() const override;
    void setCurrentPage(int newCurrentPage) override;
    int getCurrentPage() const override;
    float getZoom() const override;
    void setZoom(float newZoom) override;
    application::core::FilteredTOCModel* getTableOfContents() override;

private:
    QRectF fzQuadToQRectF(const mupdf::FzQuad& rect);

private:
    application::IBookService* m_bookService;
};

}  // namespace adapters::controllers