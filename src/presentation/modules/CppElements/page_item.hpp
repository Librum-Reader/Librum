#include <QPoint>
#include <QQuickItem>
#include <QString>
#include <QTimer>
#include <memory>
#include "document_item.hpp"
#include "page.hpp"
#include "presentation_export.hpp"

namespace cpp_elements
{

class PRESENTATION_EXPORT PageItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(
        DocumentItem* document READ getDocument WRITE setDocument CONSTANT)
    Q_PROPERTY(
        int implicitWidth READ getImplicitWidth NOTIFY implicitWidthChanged)
    Q_PROPERTY(
        int implicitHeight READ getImplicitHeight NOTIFY implicitHeightChanged)
    Q_PROPERTY(int pageNumber READ getPageNumber WRITE setPageNumber CONSTANT)
    Q_PROPERTY(bool colorInverted WRITE setColorInverted)


public:
    PageItem();

    int getImplicitWidth() const;
    int getImplicitHeight() const;

    DocumentItem* getDocument() const;
    void setDocument(DocumentItem* newDocument);
    int getPageNumber() const;
    void setPageNumber(int newCurrentPage);
    void setColorInverted(bool newColorInverted);

private slots:
    void updateZoom(float newZoom);

protected:
    void geometryChange(const QRectF& newGeometry,
                        const QRectF& oldGeometry) override;
    QSGNode* updatePaintNode(QSGNode* node, UpdatePaintNodeData* _) override;

    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void hoverMoveEvent(QHoverEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;

private:
    void selectSingleWord();
    void selectMultipleWords();
    void selectLine();
    void selectPosition(QRectF rect);
    void copySelectedText();
    void removeSelection();
    void drawSelection();
    void generateSelection();
    void paintSelectionOnPage(QPainter& painter);

    void resetCursorToDefault();
    void setCorrectCursor(int x, int y);
    void followLink(mupdf::FzLink& link);

    DocumentItem* m_document = nullptr;
    std::unique_ptr<application::core::Page> m_page;
    int m_currentPage = 0;
    bool m_firstTimeColorInverted = true;
    bool m_startedMousePressOnLink = false;
    QPointF m_selectionStart;
    QPointF m_selectionEnd;
    QTimer m_tripleClickTimer;
    bool m_doubleClickHold = false;
};

}  // namespace cpp_elements
