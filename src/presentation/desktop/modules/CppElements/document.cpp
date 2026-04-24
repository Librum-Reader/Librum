#include "document.hpp"
#include "document_access.hpp"
#include <QApplication>
#include <QDesktopServices>
#include <QPair>
#include <QRandomGenerator>
#include <QSGImageNode>
#include <QSGSimpleRectNode>
#include <cmath>

namespace cpp_elements
{

using adapters::IOpenedBookController;
using adapters::core::DocumentAccess;
using adapters::core::DocumentSearcher;
using adapters::data_models::FilteredTOCModel;
using adapters::data_models::TOCModel;

Document::Document()
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
    connect(this, &Document::contentYChanged, this,
            &Document::updateCurrentPage);

    // Ensure the content item has the same width as the document view
    connect(this, &QQuickItem::implicitWidthChanged, this,
            [this]()
            {
                m_contentItem->setWidth(implicitWidth());
                emit contentWidthChanged();
            });
    connect(m_contentItem, &QQuickItem::heightChanged, this,
            &Document::contentHeightChanged);

    // We need to find a good value for this after rendering the first
    // page I assume
    setImplicitWidth(500);
}

void Document::setOpenedBookController(
    IOpenedBookController* newOpenedBookController)
{
    m_openedBookController = newOpenedBookController;
    m_documentAccess = std::make_unique<DocumentAccess>(
        newOpenedBookController->getFilePath());

    // Document Searcher
    m_documentSeacher =
        std::make_unique<DocumentSearcher>(m_documentAccess.get());
    connect(m_documentSeacher.get(), &DocumentSearcher::goToPosition, this,
            &Document::setCurrentPageWithOffsetY);
    connect(m_documentSeacher.get(), &DocumentSearcher::selectText, this,
            [this](int pageNumber, const QRectF& rect)
            {
                auto page = m_activePages.value(pageNumber);
                if(page != nullptr)
                {
                    auto middleLeft = QPointF(rect.left(), rect.center().y());
                    auto middleRight = QPointF(rect.right(), rect.center().y());
                    page->selectTextBetweenPoints(middleLeft, middleRight);
                }
                else
                {
                    qWarning()
                        << "Search hit tried to highlight text on a page "
                           "that is not rendered yet.";
                }
            });

    m_isSetup = true;
    emit tableOfContentsChanged();

    if(!setupDefaultPageHeight())
    {
        qWarning() << "Could not determine the default page size.";
        return;
    }

    loadDefaultBookData();
    redrawPages();
}

bool Document::setupDefaultPageHeight()
{
    // We take the first page to determine the page height, not the cover
    Page page(this, m_openedBookController, m_documentAccess.get(), 1,
              m_currentZoom, window()->devicePixelRatio());

    m_pageHeight = page.getImplicitHeight();
    if(m_pageHeight == 0)
        return false;

    m_contentItem->setHeight(m_pageHeight * m_currentZoom *
                             m_openedBookController->getPageCount());
    return true;
}

void Document::loadDefaultBookData()
{
    if(m_openedBookController == nullptr)
        return;

    auto newContentY =
        getContentYForPage(m_openedBookController->getCurrentPage());
    setContentY(newContentY);
}

adapters::IDocumentAccess* Document::getDocumentAccess()
{
    if(m_documentAccess == nullptr)
    {
        qWarning() << "Document access is not set.";
        return nullptr;
    }

    return m_documentAccess.get();
}

QSGNode* Document::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
    return oldNode;
}

void Document::wheelEvent(QWheelEvent* event)
{
    int deltaY = event->angleDelta().y();
    int deltaX = event->angleDelta().x();
    if(event->modifiers() & Qt::ControlModifier)
        applyZoom(calculateNewZoom(deltaY, m_zoomFactor), ZoomMode::Mouse);
    else
        handleScroll(deltaY, deltaX);

    event->accept();
}

void Document::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    if(key == Qt::Key_Up)
    {
        handleScroll(20, 0);
    }
    else if(key == Qt::Key_Down)
    {
        handleScroll(-20, 0);
    }
    else if(key == Qt::Key_Minus)
    {
        if(event->modifiers() & Qt::ControlModifier)
        {
            applyZoom(calculateNewZoom(-15, m_zoomFactor), ZoomMode::Keyboard);
        }
    }
    else if(key == getShortcut("PageDown"))
    {
        setContentY(m_contentY + height());
    }
    // Need to handle CTRL + differently bc. it Key_Plus isn't recognized
    else if(event->key() == Qt::Key_Plus || event->nativeVirtualKey() == 187)
    {
        applyZoom(calculateNewZoom(15, m_zoomFactor), ZoomMode::Keyboard);
    }

    event->accept();
}

bool Document::event(QEvent *event)
{
    if (event->type() == QEvent::NativeGesture) {
        auto *gestureEvent = static_cast<QNativeGestureEvent *>(event);

        if (gestureEvent->gestureType() == Qt::ZoomNativeGesture) {
            handleMousepadZoom(gestureEvent);
            return true;
        }
    }

    return QQuickItem::event(event);
}

void Document::redrawPages()
{
    if(m_openedBookController == nullptr)
        return;

    setFocus(true);
    QPair<int, int> pageSpan = getPageSpanToRender();

    // Display pages
    qreal widestPage = 0;
    for(int i = pageSpan.first; i <= pageSpan.second; ++i)
    {
        Page* page = m_activePages.value(i);
        if(page == nullptr)
        {
            page = createPage(i);
            page->setIncludeNewLinesInCopiedText(m_includeNewLinesInCopiedText);
            page->setColorInverted(m_colorInverted);
            m_activePages.emplace(i, page);
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
            // deleteLater is needed, since this function can be called from
            // pages which can't be deleted directly, but must be scheduled.
            it.value()->deleteLater();
            it = m_activePages.erase(it);
        }
        else
        {
            ++it;
        }
    };

    update();
}

Page* Document::createPage(int pageNumber)
{
    Page* page =
        new Page(m_contentItem, m_openedBookController, m_documentAccess.get(),
                 pageNumber, m_currentZoom, window()->devicePixelRatio());
    page->setParentItem(m_contentItem);

    connect(page, &Page::followLink, this, &Document::followLink);
    connect(page, &Page::textSelectionFinished, this,
            &Document::textSelectionFinished);
    connect(page, &Page::highlightSelected, this, &Document::highlightSelected);
    connect(page, &Page::mouseHoverMoved, this, &Document::mouseMoved);
    connect(page, &Page::resetCursor, this,
            [this]()
            {
                if(!m_cursorHidden)
                    QApplication::restoreOverrideCursor();
            });
    connect(page, &Page::updateCursor, this,
            [this](Qt::CursorShape cursor)
            {
                if(!m_cursorHidden)
                    QApplication::setOverrideCursor(cursor);
            });

    return page;
}

QPair<int, int> Document::getPageSpanToRender()
{
    if(m_pageHeight == 0)
        return {};

    double pageStep = (m_pageHeight + m_spacing) * m_currentZoom;
    int first = m_contentY / pageStep;
    int last = (m_contentY + height()) / pageStep;

    // For the case that contentY perfectly aligns with the book
    // start (0) or end, we need to add or subtract 1
    if(last >= m_openedBookController->getPageCount())
        last = m_openedBookController->getPageCount() - 1;
    else if(m_contentY == 0)
        last += 1;

    return { first, last };
}

void Document::handleScroll(int deltaY, int deltaX)
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

void Document::moveX(int amount)
{
    int maxContentX = m_contentItem->width() - width();
    if(m_contentX + amount > maxContentX)
        m_contentX = maxContentX;
    else if(m_contentX + amount < 0)
        m_contentX = 0;
    else
        m_contentX += amount;
}

void Document::moveY(int amount)
{
    m_contentY += amount;

    ensureInBounds();
}

double Document::calculateNewZoom(double deltaY, double zoomFactor)
{
    return m_currentZoom * (deltaY > 0 ? 1 + zoomFactor : 1 - zoomFactor);
}

void Document::applyZoom(double newZoom, ZoomMode zoomMode)
{
    // Avoid excessive zooms that could cause performance issues
    if(newZoom > 10)
        newZoom = 10;
    else if(newZoom < 0.2)
        newZoom = 0.2;

    double scale = newZoom / m_currentZoom;
    m_currentZoom = newZoom;
    emit currentZoomChanged();

    if(zoomMode == ZoomMode::Keyboard)
        m_contentY = contentYForCenterZoom(scale);
    else if(zoomMode == ZoomMode::Mouse)
        m_contentY = contentYForMouseZoom(scale);

    m_contentY = m_contentY < 0 ? 0 : m_contentY; // Ensure not out of bounds

    m_contentItem->setHeight((m_pageHeight + m_spacing) *
                                 m_openedBookController->getPageCount() *
                                 m_currentZoom -
                             m_spacing);
    m_contentItem->setY(-m_contentY);
    emit contentYChanged();

    ensureInBounds();
    redrawPages();
}

double Document::contentYForCenterZoom(double scale)
{
    double centerY = m_contentY + height() / 2;
    double newCenterY = centerY * scale;
    return newCenterY - height() / 2;
}

double Document::contentYForMouseZoom(double scale)
{
    QPointF localPos = mapFromGlobal(QCursor::pos());

    double virtualMouseY = m_contentY + localPos.y();
    double newVirtualMouseY = virtualMouseY * scale;
    return newVirtualMouseY - localPos.y();
}

void Document::ensureInBounds()
{
    int totalContentHeight = (m_pageHeight + m_spacing) * m_currentZoom *
                                 m_openedBookController->getPageCount() -
                             m_spacing * m_currentZoom;
    int maxContentY = totalContentHeight - height();
    if(m_contentY > maxContentY)
        m_contentY = maxContentY;
    else if(m_contentY < 0)
        m_contentY = 0;

    m_contentItem->setY(-m_contentY);
    emit contentYChanged();
}

void Document::updateCurrentPage()
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

long Document::getContentYForPage(int page) const
{
    return page * (m_pageHeight + m_spacing) * m_currentZoom;
}

void Document::setCurrentPageWithOffsetY(int currentPage, int offsetY)
{
    if(m_currentPage == currentPage)
        return;

    if(currentPage < 0 || currentPage >= m_openedBookController->getPageCount())
        return;

    m_currentPage = currentPage;
    auto newContentY = getContentYForPage(currentPage) + offsetY;
    setContentY(newContentY);
    emit currentPageChanged();
}

void Document::handleMousepadZoom(QNativeGestureEvent* event)
{
    qreal zoomDelta = event->value();

    qreal newZoom = calculateNewZoom(zoomDelta, std::abs(zoomDelta));
    applyZoom(newZoom, ZoomMode::Mouse);

    event->accept();
}

Qt::Key Document::getShortcut(const QString& value)
{
    if(m_settingsController == nullptr)
    {
        qWarning() << "Settings controller is not set.";
        return Qt::Key_unknown;
    }

    auto shortcut =
        m_settingsController->getShortcuts()->value(value).toString();
    QKeySequence seq = QKeySequence(shortcut);
    QKeyCombination keyCombo = seq[0];
    return keyCombo.key();
}

long Document::getContentHeight() const
{
    return m_contentItem->height();
}

long Document::getContentY() const
{
    return m_contentY;
}

void Document::setContentY(long newContentY)
{
    if(m_contentY == newContentY)
        return;

    m_contentY = newContentY;
    m_contentItem->setY(-m_contentY);
    emit contentYChanged();

    ensureInBounds();
    redrawPages();
}

long Document::getContentX() const
{
    return m_contentX;
}

void Document::setContentX(long newContentX)
{
    if(m_contentX == newContentX)
        return;

    m_contentX = newContentX;
    m_contentItem->setX(-m_contentX);
    emit contentXChanged();

    redrawPages();
}

long Document::getContentWidth() const
{
    return m_contentItem->width();
}

double Document::getCurrentZoom() const
{
    return m_currentZoom;
}

void Document::setCurrentZoom(double newCurrentZoom)
{
    if(m_currentZoom == newCurrentZoom)
        return;

    applyZoom(newCurrentZoom, ZoomMode::Keyboard);
}

int Document::currentPage() const
{
    return m_currentPage;
}

void Document::setCurrentPage(int newCurrentPage)
{
    if(m_currentPage == newCurrentPage)
        return;

    if(newCurrentPage < 0 ||
       newCurrentPage >= m_openedBookController->getPageCount())
        return;

    m_currentPage = newCurrentPage;
    auto newContentY = getContentYForPage(newCurrentPage);
    setContentY(newContentY);
    emit currentPageChanged();
}

int Document::getTopPage() const
{
    if(m_activePages.empty())
        return -1;

    auto it = m_activePages.begin();
    int minValue = it.value()->getPageNumber();
    for(; it != m_activePages.end(); ++it)
    {
        if(it.value()->getPageNumber() < minValue)
        {
            minValue = it.value()->getPageNumber();
        }
    }

    return minValue;
}

int cpp_elements::Document::getTopPageYOffset() const
{
    int topPage = getTopPage();
    auto offset = (m_contentY / m_currentZoom) -
                  (getContentYForPage(topPage) / m_currentZoom);
    auto boundedOffset = qBound(0.0, offset, static_cast<double>(m_pageHeight));

    return qRound(boundedOffset);
}

void Document::followLink(const char* uri)
{
    if(m_documentAccess->linkIsExternal(uri))
    {
        QDesktopServices::openUrl(QUrl(uri));
    }
    else
    {
        auto position = m_documentAccess->getPositionOfLink(uri);
        setCurrentPageWithOffsetY(position.pageNumber,
                                  position.yOffset * m_currentZoom);
    }
}

void Document::removeSelectionFromPage(int pageNumber)
{
    auto page = m_activePages.value(pageNumber);
    if(page == nullptr)
    {
        qWarning() << "Page not rendered yet, can't remove selection.";
        return;
    }

    page->removeSelection();
}

void Document::restoreCursor()
{
    m_cursorHidden = false;
    QApplication::restoreOverrideCursor();
}

void Document::hideCursor()
{
    m_cursorHidden = true;
    QApplication::setOverrideCursor(Qt::BlankCursor);
}

adapters::data_models::FilteredTOCModel* Document::getTableOfContents()
{
    if(!m_isSetup)
        return nullptr;

    if(m_TOCModel == nullptr)
    {
        m_TOCModel = m_documentAccess->loadTOCModel();
        m_filteredTOCModel = std::make_unique<FilteredTOCModel>();
        m_filteredTOCModel->setSourceModel(m_TOCModel.get());
    }

    return m_filteredTOCModel.get();
}

adapters::IDocumentSearcher* Document::getDocumentSearcher() const
{
    return m_documentSeacher.get();
}

void Document::setSettingsController(
    adapters::ISettingsController* newSettingsController)
{
    m_settingsController = newSettingsController;
}

void Document::setColorInverted(bool newColorInverted)
{
    for(auto book : m_activePages.values())
        book->setColorInverted(newColorInverted);

    m_colorInverted = newColorInverted;
}

void Document::setIncludeNewLinesInCopiedText(bool newIncludeNewLinesInCopiedText)
{
    for(auto book : m_activePages.values())
        book->setIncludeNewLinesInCopiedText(newIncludeNewLinesInCopiedText);

    m_includeNewLinesInCopiedText = newIncludeNewLinesInCopiedText;
}

}  // namespace cpp_elements
