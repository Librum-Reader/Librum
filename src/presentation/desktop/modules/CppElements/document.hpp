#pragma once
#include <QList>
#include <QQuickItem>
#include <QSGSimpleRectNode>
#include "document_searcher.hpp"
#include "filtered_toc_model.hpp"
#include "i_document_access.hpp"
#include "i_settings_controller.hpp"
#include "page.hpp"
#include "presentation_export.hpp"
#include "toc_model.hpp"

namespace cpp_elements
{

class PRESENTATION_EXPORT Document : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(adapters::IOpenedBookController* openedBookController WRITE
                   setOpenedBookController)
    Q_PROPERTY(adapters::ISettingsController* settingsController WRITE
                   setSettingsController)
    Q_PROPERTY(
        long contentHeight READ getContentHeight NOTIFY contentHeightChanged)
    Q_PROPERTY(
        long contentWidth READ getContentWidth NOTIFY contentWidthChanged)
    Q_PROPERTY(
        long contentY READ getContentY WRITE setContentY NOTIFY contentYChanged)
    Q_PROPERTY(
        long contentX READ getContentX WRITE setContentX NOTIFY contentXChanged)
    Q_PROPERTY(adapters::data_models::FilteredTOCModel* tableOfContents READ
                   getTableOfContents NOTIFY tableOfContentsChanged)
    Q_PROPERTY(double currentZoom READ getCurrentZoom WRITE setCurrentZoom
                   NOTIFY currentZoomChanged)
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY
                   currentPageChanged)
    Q_PROPERTY(int topPage READ getTopPage CONSTANT)
    Q_PROPERTY(int topPageYOffset READ getTopPageYOffset CONSTANT)
    Q_PROPERTY(adapters::IDocumentSearcher* documentSearcher READ
                   getDocumentSearcher CONSTANT)
    Q_PROPERTY(bool colorInverted WRITE setColorInverted)
    Q_PROPERTY(bool includeNewLinesInCopiedText WRITE setIncludeNewLinesInCopiedText)


public:
    Document();
    void setOpenedBookController(
        adapters::IOpenedBookController* newOpenedBookController);
    void setSettingsController(
        adapters::ISettingsController* newSettingsController);
    adapters::IDocumentAccess* getDocumentAccess();
    adapters::data_models::FilteredTOCModel* getTableOfContents();
    adapters::IDocumentSearcher* getDocumentSearcher() const;

    long getContentHeight() const;
    long getContentY() const;
    void setContentY(long contentY);
    long getContentX() const;
    void setContentX(long newContentX);
    long getContentWidth() const;
    double getCurrentZoom() const;
    void setCurrentZoom(double newCurrentZoom);
    int currentPage() const;
    void setCurrentPage(int newCurrentPage);
    int getTopPage() const;
    int getTopPageYOffset() const;
    void followLink(const char* uri);

    Q_INVOKABLE void removeSelectionFromPage(int pageNumber);
    Q_INVOKABLE void restoreCursor();
    Q_INVOKABLE void hideCursor();

    void setColorInverted(bool newColorInverted);
    void setIncludeNewLinesInCopiedText(bool newIncludeNewLinesInCopiedText);

signals:
    void mouseMoved(double x, double y);
    void contentHeightChanged();
    void contentYChanged();
    void contentXChanged();
    void contentWidthChanged();
    void currentZoomChanged();
    void currentPageChanged();
    void tableOfContentsChanged();
    void textSelectionFinished(float centerX, float topY);
    void highlightSelected(float centerX, float topY, const QString& uuid);

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    bool event(QEvent* event) override;

private:
    enum class ZoomMode
    {
        Keyboard,
        Mouse
    };

private:
    void redrawPages();
    QPair<int, int> getPageSpanToRender();
    bool setupDefaultPageHeight();
    Page* createPage(int pageNumber);
    void handleScroll(int deltaY, int deltaX);
    void moveX(int amount);
    void moveY(int amount);
    void applyZoom(double zoom, ZoomMode zoomMode);
    double calculateNewZoom(double deltaY, double zoomFactor);
    double contentYForCenterZoom(double scale);
    double contentYForMouseZoom(double scale);
    void ensureInBounds();
    void loadDefaultBookData();
    void updateCurrentPage();
    long getContentYForPage(int page) const;
    void setCurrentPageWithOffsetY(int currentPage, int offsetY);
    void handleMousepadZoom(QNativeGestureEvent* event);
    Qt::Key getShortcut(const QString& value);

    adapters::IOpenedBookController* m_openedBookController = nullptr;
    adapters::ISettingsController* m_settingsController = nullptr;
    std::unique_ptr<adapters::IDocumentAccess> m_documentAccess;
    std::unique_ptr<adapters::core::DocumentSearcher> m_documentSeacher;
    QHash<int, Page*> m_activePages;
    QQuickItem* m_contentItem;
    bool m_cursorHidden = false;
    bool m_isSetup = false;
    int m_pageHeight = 0;
    long m_contentY = 0;
    long m_contentX = 0;
    double m_currentZoom = 1;
    double m_zoomFactor = 0.13;
    int m_spacing = 5;
    int m_currentPage = 0;

    // Table of Contents
    std::unique_ptr<adapters::data_models::FilteredTOCModel> m_filteredTOCModel;
    std::unique_ptr<adapters::data_models::TOCModel> m_TOCModel;
    bool m_colorInverted;
    bool m_includeNewLinesInCopiedText;
};

}  // namespace cpp_elements
