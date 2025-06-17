#pragma once
#include <QList>
#include <QQuickItem>
#include <QSGSimpleRectNode>
#include "page_view.hpp"
#include "presentation_export.hpp"

namespace cpp_elements
{

class PRESENTATION_EXPORT DocumentView : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(adapters::IBookController* bookController WRITE setBookController
                   CONSTANT)
    Q_PROPERTY(
        long contentHeight READ getContentHeight NOTIFY contentHeightChanged)
    Q_PROPERTY(
        long contentWidth READ getContentWidth NOTIFY contentWidthChanged)
    Q_PROPERTY(
        long contentY READ getContentY WRITE setContentY NOTIFY contentYChanged)
    Q_PROPERTY(
        long contentX READ getContentX WRITE setContentX NOTIFY contentXChanged)
    Q_PROPERTY(double currentZoom READ getCurrentZoom WRITE setCurrentZoom
                   NOTIFY currentZoomChanged)
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY
                   currentPageChanged)

public:
    DocumentView();
    void setBookController(adapters::IBookController* newBookController);
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

signals:
    void contentHeightChanged();
    void contentYChanged();
    void contentXChanged();
    void contentWidthChanged();
    void currentZoomChanged();
    void currentPageChanged();

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

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
    void handleScroll(int deltaY, int deltaX);
    void moveX(int amount);
    void moveY(int amount);
    void applyZoom(double zoom, ZoomMode zoomMode);
    double calculateNewZoom(int deltaY);
    double contentYForCenterZoom(double scale);
    double contentYForMouseZoom(double scale);
    void ensureInBounds();
    void loadDefaultBookData();
    void updateCurrentPage();
    long getContentYForPage(int page) const;

    adapters::IBookController* m_bookController = nullptr;
    QHash<int, PageView*> m_activePages;
    QQuickItem* m_contentItem;
    int m_pageHeight = 0;
    long m_contentY = 0;
    long m_contentX = 0;
    double m_currentZoom = 1;
    double m_zoomFactor = 0.13;
    int m_spacing = 5;
    int m_currentPage = 0;
};

}  // namespace cpp_elements
