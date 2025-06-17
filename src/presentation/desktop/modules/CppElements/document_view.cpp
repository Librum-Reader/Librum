#include "document_view.hpp"
#include <QPair>
#include <QRandomGenerator>
#include <QSGImageNode>
#include <QSGSimpleRectNode>

namespace cpp_elements
{

DocumentView::DocumentView()
{
    setFlag(QQuickItem::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setFlag(QQuickItem::ItemAcceptsInputMethod, true);

    m_contentItem = new QQuickItem(this);
    m_contentItem->setParentItem(this);
    m_contentItem->setY(0);

    // Redraw the page when the height changes
    connect(this, &QQuickItem::heightChanged, this,
            [this]()
            {
                ensureInBounds();
                redrawPages();
            });

    // Update the current page when the content Y changes
    connect(this, &DocumentView::contentYChanged, this,
            &DocumentView::updateCurrentPage);

    // Ensure the content item has the same width as the document view
    connect(this, &QQuickItem::implicitWidthChanged, this,
            [this]()
            {
                m_contentItem->setWidth(implicitWidth());
                emit contentWidthChanged();
            });
    connect(m_contentItem, &QQuickItem::heightChanged, this,
            &DocumentView::contentHeightChanged);

    // We need to find a good value for this after rendering the first
    // page I assume
    setImplicitWidth(500);
}

void DocumentView::setBookController(
    adapters::IBookController* newBookController)
{
    m_bookController = newBookController;

    if(!setupDefaultPageHeight())
    {
        qWarning() << "Could not determine the default page size.";
        return;
    }

    loadDefaultBookData();
    redrawPages();
}

void DocumentView::loadDefaultBookData()
{
    if(m_bookController == nullptr)
        return;

    auto newContentY = getContentYForPage(m_bookController->getCurrentPage());
    setContentY(newContentY);
}

QSGNode* DocumentView::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
    return oldNode;
}

void DocumentView::wheelEvent(QWheelEvent* event)
{
    int deltaY = event->angleDelta().y();
    int deltaX = event->angleDelta().x();
    if(event->modifiers() & Qt::ControlModifier)
        applyZoom(calculateNewZoom(deltaY), ZoomMode::Mouse);
    else
        handleScroll(deltaY, deltaX);

    event->accept();
}

void DocumentView::keyPressEvent(QKeyEvent* event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        handleScroll(20, 0);
        break;
    case Qt::Key_Down:
        handleScroll(-20, 0);
        break;
    case Qt::Key_Minus:
        if(event->modifiers() & Qt::ControlModifier)
            applyZoom(calculateNewZoom(-15), ZoomMode::Keyboard);
        break;
    default:
        break;
    }

    // Need to handle CTRL + differently bc. it Key_Plus isn't recognized
    if(event->key() == Qt::Key_Plus || event->nativeVirtualKey() == 187)
    {
        applyZoom(calculateNewZoom(15), ZoomMode::Keyboard);
    }

    event->accept();
}

void DocumentView::redrawPages()
{
    setFocus(true);
    QPair<int, int> pageSpan = getPageSpanToRender();

    // Display pages
    qreal widestPage = 0;
    for(int i = pageSpan.first; i <= pageSpan.second; ++i)
    {
        PageView* page = m_activePages.value(i);
        if(page == nullptr)
        {
            page = new PageView(m_contentItem, m_bookController, i,
                                m_currentZoom, window()->devicePixelRatio());
            page->setParentItem(m_contentItem);
            m_activePages.tryEmplace(i, page);
        }

        page->setY(i * (m_pageHeight + m_spacing) * m_currentZoom);
        page->setZoom(m_currentZoom);
        page->setWidth(page->getImplicitWidth());
        page->setHeight(page->getImplicitHeight());

        if(page->width() > widestPage)
            widestPage = page->width();
    }

    // Adjust the window width
    if(widestPage != implicitWidth())
        setImplicitWidth(widestPage);

    // Delete unused pages
    for(auto it = m_activePages.begin(); it != m_activePages.end();)
    {
        if(it.key() < pageSpan.first || it.key() > pageSpan.second)
        {
            delete it.value();
            it = m_activePages.erase(it);
        }
        else
        {
            ++it;
        }
    };

    update();
}

bool DocumentView::setupDefaultPageHeight()
{
    // We take the first page to determine the page height, not the cover
    auto page = std::make_unique<PageView>(
        this, m_bookController, 1, m_currentZoom, window()->devicePixelRatio());
    m_pageHeight = page->getImplicitHeight();
    if(m_pageHeight == 0)
        return false;

    m_contentItem->setHeight(m_pageHeight * m_currentZoom *
                             m_bookController->getPageCount());
    return true;
}

QPair<int, int> DocumentView::getPageSpanToRender()
{
    if(m_pageHeight == 0)
        return {};

    double pageStep = (m_pageHeight + m_spacing) * m_currentZoom;
    int first = m_contentY / pageStep;
    int last = (m_contentY + height()) / pageStep;

    // For the case that contentY perfectly aligns with the book
    // start (0) or end, we need to add or subtract 1
    if(last >= m_bookController->getPageCount())
        last = m_bookController->getPageCount() - 1;
    else if(m_contentY == 0)
        last += 1;

    return { first, last };
}

void DocumentView::handleScroll(int deltaY, int deltaX)
{
    // To get the correct scroll direction
    deltaY *= -1;
    deltaX *= -1;

    moveX(deltaX);
    m_contentItem->setX(-m_contentX);
    emit contentXChanged();

    moveY(deltaY);
    m_contentItem->setY(-m_contentY);
    emit contentYChanged();

    redrawPages();
}

void DocumentView::moveX(int amount)
{
    int maxContentX = m_contentItem->width() - width();
    if(m_contentX + amount > maxContentX)
        m_contentX = maxContentX;
    else if(m_contentX + amount < 0)
        m_contentX = 0;
    else
        m_contentX += amount;
}

void DocumentView::moveY(int amount)
{
    m_contentY += amount;

    ensureInBounds();
}

double DocumentView::calculateNewZoom(int deltaY)
{
    return m_currentZoom * (deltaY > 0 ? 1 + m_zoomFactor : 1 - m_zoomFactor);
}

void DocumentView::applyZoom(double newZoom, ZoomMode zoomMode)
{
    double scale = newZoom / m_currentZoom;
    m_currentZoom = newZoom;
    emit currentZoomChanged();

    if(zoomMode == ZoomMode::Keyboard)
        m_contentY = contentYForCenterZoom(scale);
    else if(zoomMode == ZoomMode::Mouse)
        m_contentY = contentYForMouseZoom(scale);

    if(m_contentY < 0)
        m_contentY = 0;

    m_contentItem->setHeight((m_pageHeight + m_spacing) *
                                 m_bookController->getPageCount() *
                                 m_currentZoom -
                             m_spacing);
    m_contentItem->setY(-m_contentY);
    emit contentYChanged();

    ensureInBounds();
    redrawPages();
}

double DocumentView::contentYForCenterZoom(double scale)
{
    double centerY = m_contentY + height() / 2;
    double newCenterY = centerY * scale;
    return newCenterY - height() / 2;
}

double DocumentView::contentYForMouseZoom(double scale)
{
    QPointF localPos = mapFromGlobal(QCursor::pos());

    double virtualMouseY = m_contentY + localPos.y();
    double newVirtualMouseY = virtualMouseY * scale;
    return newVirtualMouseY - localPos.y();
}

void DocumentView::ensureInBounds()
{
    int totalContentHeight = (m_pageHeight + m_spacing) * m_currentZoom *
                                 m_bookController->getPageCount() -
                             m_spacing * m_currentZoom;
    int maxContentY = totalContentHeight - height();
    if(m_contentY > maxContentY)
        m_contentY = maxContentY;
    else if(m_contentY < 0)
        m_contentY = 0;

    m_contentItem->setY(-m_contentY);
    emit contentYChanged();
}

void DocumentView::updateCurrentPage()
{
    // The current page is the one at the middle of the screen
    double pageStep = (m_pageHeight + m_spacing) * m_currentZoom;
    int currentPage = (m_contentY + height() / 2) / pageStep;
    if(currentPage != m_currentPage)
    {
        m_currentPage = currentPage;
        emit currentPageChanged();
    }
}

long DocumentView::getContentYForPage(int page) const
{
    return page * (m_pageHeight + m_spacing) * m_currentZoom;
}

long DocumentView::getContentHeight() const
{
    return m_contentItem->height();
}

long DocumentView::getContentY() const
{
    return m_contentY;
}

void DocumentView::setContentY(long newContentY)
{
    if(m_contentY == newContentY)
        return;

    m_contentY = newContentY;
    m_contentItem->setY(-m_contentY);
    emit contentYChanged();

    ensureInBounds();
    redrawPages();
}

long DocumentView::getContentX() const
{
    return m_contentX;
}

void DocumentView::setContentX(long newContentX)
{
    if(m_contentX == newContentX)
        return;

    m_contentX = newContentX;
    m_contentItem->setX(-m_contentX);
    emit contentXChanged();

    redrawPages();
}

long DocumentView::getContentWidth() const
{
    return m_contentItem->width();
}

double DocumentView::getCurrentZoom() const
{
    return m_currentZoom;
}

void DocumentView::setCurrentZoom(double newCurrentZoom)
{
    if(m_currentZoom == newCurrentZoom)
        return;

    applyZoom(newCurrentZoom, ZoomMode::Keyboard);
}

int DocumentView::currentPage() const
{
    return m_currentPage;
}

void DocumentView::setCurrentPage(int newCurrentPage)
{
    if(m_currentPage == newCurrentPage)
        return;

    if(newCurrentPage < 0 || newCurrentPage >= m_bookController->getPageCount())
        return;

    m_currentPage = newCurrentPage;
    auto newContentY = getContentYForPage(newCurrentPage);
    setContentY(newContentY);
    emit currentPageChanged();
}

}  // namespace cpp_elements
