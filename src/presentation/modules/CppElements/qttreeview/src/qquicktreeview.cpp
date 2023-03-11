/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of TreeView.
**
** $QT_BEGIN_LICENSE:GPL-MARKETPLACE-QT$
**
** Marketplace License Usage
** Users, who have licensed the Software under the Qt Marketplace license
** agreement, may use this file in accordance with the Qt Marketplace license
** agreement provided with the Software or, alternatively, in accordance with
** the terms contained in a written agreement between the licensee and The Qt
** Company. For licensing terms and conditions see
** https://www.qt.io/terms-conditions/#marketplace and
** https://www.qt.io/terms-conditions. For further information use the contact
** form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/qobject.h>
#include <QtQml/qqmlcontext.h>

#include "qquicktreemodeladaptor_p.h"
#include "qquicktreeview_p_p.h"

/*!
    \qmltype TreeView
    \inqmlmodule QtQuick.TreeView
    \since 5.15
    \inherits TableView
    \brief Provides a tree view to display data from a QAbstractItemModel.
*/

/*!
    \qmlproperty int QtQuick::TreeView::currentIndex

    This property holds the view index that has been selected as current. Selecting
    an index to be current is done by either using the arrow keys, clicking on a row
    (or a cell) with the mouse, or assign \l currentIndex a value explicitly.

    \note \c currentIndex is an index into the the view model. In this model, all
    tree nodes that are visible in the view are flattened into a list that can be
    shown in a table column.
    This also means that currentIndex.row is the table row relative to root node.
    To get the current index in the model, where the row is relative to parent
    node, use \l currentModelIndex or \l mapToModel():

    \code
    var indexInModel = currentModelIndex
    var indexInModelMapped = mapToModel(currentIndex)
    console.assert(indexInModel == indexInModelMapped)
    \endcode

    If no index has been selected as current, or if \l currentModelIndex is
    is not visible in the view, currentIndex.valid will be \c false.

    \sa currentModelIndex, currentItem, mapToModel()
*/

/*!
    \qmlproperty int QtQuick::TreeView::currentModelIndex

    This property holds the model index that has been selected as current. Selecting
    an index to be current is done by either using the arrow keys, clicking on a row
    (or a cell) with the mouse, or assign \l currentIndex a value explicitly.

    \note \c currentModelIndex is an index into the the model. This also means
    that currentModelIndex.row is the child number relative to the parent node.
    To get the current row in the view, which is relative to the
    root, use \l currentIndex or \l mapFromModel():

    \code
    var indexInView = currentViewIndex
    var indexInViewMapped = mapFromModel(currentModelIndex)
    console.assert(indexInView == indexInViewMapped)
    \endcode

    If no index has been selected as current, currentModelIndex.valid will be \c false.

    \sa currentIndex, currentItem, mapFromModel()
*/

/*!
    \qmlproperty int QtQuick::TreeView::currentItem
    \readonly

    This property holds the item that is selected as current. Selecting
    an item to be current is done by either using the arrow keys, clicking on a row
    (or a cell) with the mouse, or assign a value to \l currentIndex explicitly.

    \note Using \l currentItem is \e almost the same as calling
    \c {itemAtModelIndex(currentModelIndex)}. The difference is that the latter
    expression will only change when \l currentModelIndex changes. But \l currentItem
    can also change as a result of the view doing a relayout behind the
    scenes (which can force delegate items to be reloaded or recycled).
    Since the item can be recycled or destroyed at any point in time behind the
    applications back, you should never store this value.

    if \c currentIndex.valid is \c false, this property will be \c null.

    \sa itemAtCell(), currentIndex, currentModelIndex, itemAtModelIndex()
*/

/*!
    \qmlproperty int QtQuick::TreeView::navigationMode

    This property holds how you would like the user to be able
    navigate the tree. It can have the following values:

    \value TreeView.List The tree view acts like as a list view. This means that
        the user can only navigate up and down in the tree, but not sideways to
        the other columns. In this mode the left and right arrow keys will collapse
        and expand nodes in the tree (in addition to \c Qt.Key_Space).
    \value TreeView.Table The tree view acts like a table view. This means that
        the user can navigate up and down, but also left and right through
        all the columns in the tree. In this mode the left and right arrow keys will
        move currentIndex left and right, and only \c Qt.Key_Space will toggle
        nodes to be collapsed or expanded.

    \note the selected mode can also affect how the tree is styled by the delegate.
*/

/*!
    \qmlproperty int QtQuick::TreeView::styleHints

    This property holds a set of hints that can be used to tweak the
    appearance of the delegate without the need to create a custom one.
    The hints that can be set is the following:

    \value styleHints.indicator The color of the collapsed/expanded icon
    \value styleHints.indicatorCurrent The color of the current rows collapsed/expanded icon
    \value styleHints.indicatorHovered The color of the collapsed/expanded icon when hovered
    \value styleHints.overlay The color of the overlay used for \l currentIndex
    \value styleHints.overlayHovered The color of the overlay used for \l currentIndex when hovered
    \value styleHints.foregroundOdd The foreground color of odd rows
    \value styleHints.backgroundOdd The background color of odd rows
    \value styleHints.foregroundEven The foreground color of even rows
    \value styleHints.backgroundEven The background color of even rows
    \value styleHints.foregroundCurrent The foreground color of the current item (or
        the whole row, if \l navigationMode is \c TreeView.List)
    \value styleHints.backgroundCurrent The background color of the current item (or
        the whole row, if \l navigationMode is \c TreeView.List)
    \value styleHints.foregroundHovered The foreground color of the current item when hovered (or
        the whole row, if \l navigationMode is \c TreeView.List)
    \value styleHints.backgroundHovered The background color of the current item when hovered (or
        the whole row, if \l navigationMode is \c TreeView.List)
    \value styleHints.indent The horizontal space between a parent and a child node
    \value styleHints.columnPadding The padding between a cell and its contents
    \value styleHints.font The font used by text items in the delegate

    \note a delegate is free to ignore any hints specified by the application.
*/

/*!
    \qmlmethod bool QtQuick::TreeView::hasChildren(row)

    Returns if the given \a row in the view is shown as
    expanded.

    \note \a row should be the row in the view, not in the model.

    \sa viewIndex
*/

/*!
    \qmlmethod bool QtQuick::TreeView::hasSiblings(row)

    Returns if the given \a row in the view has
    siblings.

    \note \a row should be the row in the view, not in the model.

    \sa viewIndex
*/

/*!
    \qmlmethod int QtQuick::TreeView::depth(row)

    Returns the depth (the number of parents up to
    the root) of the given \a row.

    \note \a row should be the row in the view, not in the model.

    \sa viewIndex
*/

/*!
    \qmlmethod bool QtQuick::TreeView::isExpanded(row)

    Returns if the given \a row in the view is shown as
    expanded.

    \note \a row should be the row in the view, not in the model.

    \sa viewIndex
*/

/*!
    \qmlmethod QtQuick::TreeView::expand(row)

    Expands the tree node at the
    given \a row in the view.

    \note this function will not affect the model, only
    the visual representation in the view.

    \sa viewIndex, collapse(), expandModelIndex(), collapseModelIndex(), isExpanded()
*/

/*!
    \qmlmethod QtQuick::TreeView::collapse(row)

    Collapses the tree node at the
    given \a row in the view.

    \note this function will not affect the model, only
    the visual representation in the view.

    \sa viewIndex, expand(), expandModelIndex(), collapseModelIndex(), isExpanded()
*/

/*!
    \qmlmethod QtQuick::TreeView::toggleExpanded(row)

    Toggles if the tree node at the given \a row
    should be expanded. This is a convenience for doing:

    \code
    if (isExpanded(row))
        collapse(row)
    else
        expand(row)
    \endcode
*/

/*!
    \qmlmethod bool QtQuick::TreeView::isModelIndexExpanded(modelIndex)

    Returns \c true if the given \a modelIndex is shown as expanded
    in the view. This is a convenience for doing:

    \code
    isExpanded(mapFromModel(modelIndex).row)
    \endcode

    \sa viewIndex
*/

/*!
    \qmlmethod QtQuick::TreeView::collapseModelIndex(modelIndex)

    Collapses the tree node at the
    given \a modelIndex. This is a convenience for doing:

    \code
    collapse(mapFromModel(modelIndex).row)
    \endcode

    \sa viewIndex, collapse(), expand(), expandModelIndex(), isExpanded()
*/

/*!
    \qmlmethod QtQuick::TreeView::expandModelIndex(modelIndex)

    Expands the tree node at the
    given \a modelIndex. This is a convenience for doing:

    \code
    expand(mapFromModel(modelIndex).row)
    \endcode

    \sa viewIndex, collapse(), expand(), collapseModelIndex(), isExpanded()
*/

/*!
    \qmlmethod QtQuick::TreeView::toggleModelIndexExpanded(modelIndex)

    Toggles if the tree node at the given \a modelIndex
    should be expanded. This is a convenience for doing:

    \code
    if (isModelIndexExpanded(modelIndex))
        collapseModelIndex(modelIndex)
    else
        expandModelIndex(modelIndex)
    \endcode
*/

/*!
    \qmlmethod int QtQuick::TreeView::columnAtX(x, includeSpacing)

    Returns the column under \a x in view coordinates.

    If \a includeSpacing is set to \c false, and \a x is on top
    of the spacing between the columns, the return value will be -1.
    Otherwise, if \a x is on top of the spacing and \a includeSpacing
    is set to \c true, returns the column closest to the position.
*/

/*!
    \qmlmethod int QtQuick::TreeView::rowAtY(y, includeSpacing)

    Returns the row under \a y in view coordinates.

    If \a includeSpacing is set to \c false, and \a y is on top
    of the spacing between the rows, returns -1.
    Otherwise, if \a y is on top of the spacing and \a includeSpacing
    is set to \c true, returns the row closest to the position.
*/

/*!
    \qmlmethod Item QtQuick::TreeView::itemAtCell(point cell)

    Returns the item inside the given table \a cell. If the cell
    is not visible in the view, returns \c null.

    \sa currentItem, itemAtIndex()
*/

/*!
    \qmlmethod Item QtQuick::TreeView::itemAtIndex(viewIndex)

    Returns the item at the given view index \a viewIndex. If
    \c viewIndex.valid is \c false, returns \c null.

    \sa currentItem, itemAtCell(), viewIndex(), itemAtModelIndex()
*/

/*!
    \qmlmethod Item QtQuick::TreeView::itemAtModelIndex(modelIndex)

    Returns the item at the given model index \a modelIndex. If the
    item that represents the model index is not visible in the view,
    returns \c null. Convenience for doing:

    \code
    itemAtIndex(mapFromModel(modelIndex))
    \endcode

    \sa currentItem, itemAtCell(), viewIndex(), itemAtIndex()
*/

/*!
    \qmlmethod QModelIndex QtQuick::TreeView::viewIndex(column, row)

    Creates a model index into the view model from the given
    \a row and \a column.

    \note this index is only valid for use with the view. To create
    a model index to use with the model, refer to the documentation for
    \l QAbstractItemModel.
*/

/*!
    \qmlmethod QModelIndex QtQuick::TreeView::mapToModel(viewIndex)

    Maps \a viewIndex, an index pointing to an item in the view (the
    view model), to the corresponding model index in the model.

    TreeView uses a view model internally to convert a tree model into
    a model suited to be shown in a \l TableView. This view model will
    flatten the parts of the tree model that at any point in time is visible
    inside view, to a list. Depending on what you need to do, it's sometimes
    easier to work with model indexes rather than view indexes, and vice-versa.

    \sa mapFromModel()
*/

/*!
    \qmlmethod QModelIndex QtQuick::TreeView::mapFromModel(modelIndex)

    Maps an index, \a modelIndex, pointing to an item in the model to the
    corresponding model index in the view (the view model).

    TreeView uses a view model internally to convert a tree model into
    a model suited to be shown in a \l TableView. This view model will
    flatten the parts of the tree model that at any point in time is
    visible inside view, to a list. Depending on what you need to
    do, it's sometimes easier to work with model indexes rather than
    view indexes, and vice-versa.

    \sa mapToModel()
*/

/*!
    \qmlattachedproperty TreeView TreeView::view

    This attached property holds the view that manages the delegate instance.
    It is attached to each instance of the delegate.
*/

/*!
    \qmlattachedproperty bool TreeView::hasChildren

    This attached property holds if the delegate instance is to be shown as
    having children (the model index represented by the instance has children
    in the model). It is attached to each instance of the delegate.
*/

/*!
    \qmlattachedproperty bool TreeView::isExpanded

    This attached property holds if the delegate instance is to be shown as
    expanded (the view index represented by the instance is expanded in the view model).
    It is attached to each instance of the delegate.
*/

/*!
    \qmlattachedproperty int TreeView::depth

    This attached property holds the tree depth (the number of parents up to the root)
    of the delegate instance. It is attached to each instance of the delegate.
*/

QT_BEGIN_NAMESPACE

QQuickTreeViewPrivate::QQuickTreeViewPrivate()
    : QQuickTableViewPrivate()
{
}

QQuickTreeViewPrivate::~QQuickTreeViewPrivate()
{
}

QVariant QQuickTreeViewPrivate::modelImpl() const
{
    return m_assignedModel;
}

void QQuickTreeViewPrivate::setModelImpl(const QVariant &newModel)
{
    Q_Q(QQuickTreeView);

    if (newModel == m_assignedModel)
        return;

    m_assignedModel = newModel;
    QVariant effectiveModel = m_assignedModel;
    if (effectiveModel.userType() == qMetaTypeId<QJSValue>())
        effectiveModel = effectiveModel.value<QJSValue>().toVariant();

    if (effectiveModel.isNull())
        m_proxyModel.setModel(nullptr);
    else if (const auto qaim = qvariant_cast<QAbstractItemModel*>(effectiveModel))
        m_proxyModel.setModel(qaim);
    else
        qmlWarning(q) << "treeView only accept models of type QAbstractItemModel";


    scheduleRebuildTable(QQuickTableViewPrivate::RebuildOption::All);
    emit q->modelChanged();
}

void QQuickTreeViewPrivate::syncModel()
{
    if (model) {
        QObjectPrivate::disconnect(model, &QQmlInstanceModel::initItem, this, &QQuickTreeViewPrivate::initItemCallback);
        QObjectPrivate::disconnect(model, &QQmlInstanceModel::itemReused, this, &QQuickTreeViewPrivate::itemReusedCallback);
    }

    QQuickTableViewPrivate::syncModel();

    if (model) {
        QObjectPrivate::connect(model, &QQmlInstanceModel::initItem, this, &QQuickTreeViewPrivate::initItemCallback);
        QObjectPrivate::connect(model, &QQmlInstanceModel::itemReused, this, &QQuickTreeViewPrivate::itemReusedCallback);
    }
}

QQuickTreeViewAttached *QQuickTreeViewPrivate::getAttachedObject(const QObject *object) const
{
    QObject *attachedObject = qmlAttachedPropertiesObject<QQuickTreeView>(object);
    return static_cast<QQuickTreeViewAttached *>(attachedObject);
}

void QQuickTreeViewPrivate::dataChangedCallback(
        const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(roles);

    for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
        for (int column = topLeft.column(); column <= bottomRight.column(); ++column) {
            const QPoint cell(column, row);
            auto item = itemAtCell(cell);
            if (!item)
                continue;

            updateAttachedProperties(item);
        }
    }
}

void QQuickTreeViewPrivate::initItemCallback(int modelIndex, QObject *object)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
    Q_UNUSED(modelIndex);
#else
    QQuickTableViewPrivate::initItemCallback(modelIndex, object);
#endif
    updateAttachedProperties(object);
}

void QQuickTreeViewPrivate::itemReusedCallback(int modelIndex, QObject *object)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
    Q_UNUSED(modelIndex);
#else
    QQuickTableViewPrivate::itemReusedCallback(modelIndex, object);
#endif
    updateAttachedProperties(object);
}

void QQuickTreeViewPrivate::updateAttachedProperties(QObject *object)
{
    Q_Q(QQuickTreeView);
    auto attached = getAttachedObject(object);
    if (!attached)
        return;

    const auto context = qmlContext(object);
    const int row = context->contextProperty(QStringLiteral("row")).toInt();
    attached->setView(q);
    attached->setIsExpanded(q->isExpanded(row));
    attached->setHasChildren(q->hasChildren(row));
    attached->setDepth(m_proxyModel.depthAtRow(row));
}

void QQuickTreeViewPrivate::updatePolish()
{
    QQuickTableViewPrivate::updatePolish();
    if (loadRequest.isActive())
        return;

    checkForPropertyChanges();
}

void QQuickTreeViewPrivate::checkForPropertyChanges()
{
    Q_Q(QQuickTreeView);

    if (m_currentModelIndexEmitted != m_currentModelIndex) {
        // m_currentIndex is a QPersistentModelIndex which will update automatically, so
        // we need this extra detour to check if is has changed after a model change.
        m_currentModelIndexEmitted = m_currentModelIndex;
        emit q->currentIndexChanged();
        emit q->currentModelIndexChanged();
    }

    QQuickItem *currentItem = q->currentItem();
    if (m_currentItemEmitted != currentItem) {
        // Because QQuickTableView shuffles and reuse items, we need to check each time
        // after a rebuild if currentItem has changed, and not trust that this only changes
        // when m_currentViewIndex changes.
        m_currentItemEmitted = currentItem;
        emit q->currentItemChanged();
    }
}

QQuickItem *QQuickTreeViewPrivate::itemAtCell(const QPoint &cell) const
{
    // (copy from qquicktableview in Qt6)
    const int modelIndex = modelIndexAtCell(cell);
    if (!loadedItems.contains(modelIndex))
        return nullptr;
    return loadedItems.value(modelIndex)->item;
}

qreal QQuickTreeViewPrivate::effectiveRowHeight(int row) const
{
    // (copy from qquicktableview in Qt6)
    return loadedTableItem(QPoint(leftColumn(), row))->geometry().height();
}

qreal QQuickTreeViewPrivate::effectiveColumnWidth(int column) const
{
    // (copy from qquicktableview in Qt6)
    return loadedTableItem(QPoint(column, topRow()))->geometry().width();
}

void QQuickTreeViewPrivate::moveCurrentViewIndex(int directionX, int directionY)
{
    Q_Q(QQuickTreeView);
    if (loadedItems.isEmpty())
        return;

    const QModelIndex oldViewIndex = q->mapFromModel(m_currentModelIndex);
    const int row = qBound(0, oldViewIndex.row() + directionY, q->rows() - 1);
    const int column = qBound(0, oldViewIndex.column() + directionX, q->columns() - 1);
    const QModelIndex newViewIndex = q->viewIndex(column, row);
    q->setCurrentModelIndex(q->mapToModel(newViewIndex));

    // Move the current item into the viewport so that it loads
    if (column < leftColumn())
        q->setContentX(q->contentX() - cellSpacing.width() - 1);
    else if (column > rightColumn())
        q->setContentX(q->contentX() + cellSpacing.width() + 1);

    if (row < topRow())
        q->setContentY(q->contentY() - cellSpacing.height() - 1);
    else if (row > bottomRow())
        q->setContentY(q->contentY() + cellSpacing.height() + 1);

    if (QQuickItem *item = q->currentItem()) {
        // Ensure that the whole item is visible
        const QRectF itemRect = QRectF(q->mapFromItem(q->contentItem(), item->position()), item->size());

        if (itemRect.x() <= 0)
            q->setContentX(item->x());
        else if (itemRect.right() > q->size().width())
            q->setContentX(item->x() + item->width() - q->width());

        if (itemRect.y() <= 0)
            q->setContentY(item->y());
        else if (itemRect.bottom() > q->size().height())
            q->setContentY(item->y() + item->height() - q->height());
    }
}

QQuickTreeView::QQuickTreeView(QQuickItem *parent)
    : QQuickTableView(*(new QQuickTreeViewPrivate), parent)
{
    Q_D(QQuickTreeView);

    // QQuickTableView will only ever see the proxy model
    const auto proxy = QVariant::fromValue(std::addressof(d->m_proxyModel));
    d->QQuickTableViewPrivate::setModelImpl(proxy);
    QObjectPrivate::connect(&d->m_proxyModel, &QAbstractItemModel::dataChanged,
                            d, &QQuickTreeViewPrivate::dataChangedCallback);
}

QQuickTreeView::~QQuickTreeView()
{
}

bool QQuickTreeView::isExpanded(int row) const
{
    Q_D(const QQuickTreeView);
    if (row < 0 || row >= d->m_proxyModel.rowCount())
        return false;

    return d->m_proxyModel.isExpanded(row);
}

bool QQuickTreeView::hasChildren(int row) const
{
    Q_D(const QQuickTreeView);
    if (row < 0 || row >= d->m_proxyModel.rowCount())
        return false;

    return d->m_proxyModel.hasChildren(row);
}

bool QQuickTreeView::hasSiblings(int row) const
{
    Q_D(const QQuickTreeView);
    if (row < 0 || row >= d->m_proxyModel.rowCount())
        return false;

    return d->m_proxyModel.hasSiblings(row);
}

int QQuickTreeView::depth(int row) const
{
    Q_D(const QQuickTreeView);
    if (row < 0 || row >= d->m_proxyModel.rowCount())
        return -1;

    return d->m_proxyModel.depthAtRow(row);
}

void QQuickTreeView::expand(int row)
{
    Q_D(QQuickTreeView);
    if (row < 0 || row >= d->m_proxyModel.rowCount())
        return;

    if (d->m_proxyModel.isExpanded(row))
        return;

    d->m_proxyModel.expandRow(row);

    if (const auto delegateItem = d->itemAtCell(QPoint(0, row))) {
        if (auto attached = d->getAttachedObject(delegateItem))
            attached->setIsExpanded(true);
    }

    emit expanded(row);
}

void QQuickTreeView::expandModelIndex(const QModelIndex &modelIndex)
{
    expand(mapFromModel(modelIndex).row());
}

void QQuickTreeView::collapseModelIndex(const QModelIndex &modelIndex)
{
    collapse(mapFromModel(modelIndex).row());
}

void QQuickTreeView::toggleModelIndexExpanded(const QModelIndex &modelIndex)
{
    toggleExpanded(mapFromModel(modelIndex).row());
}

void QQuickTreeView::collapse(int row)
{
    Q_D(QQuickTreeView);
    if (row < 0 || row >= d->m_proxyModel.rowCount())
        return;

    if (!d->m_proxyModel.isExpanded(row))
        return;

    d_func()->m_proxyModel.collapseRow(row);

    if (const auto delegateItem = d->itemAtCell(QPoint(0, row))) {
        if (auto attached = d->getAttachedObject(delegateItem))
            attached->setIsExpanded(false);
    }

    emit collapsed(row);
}

void QQuickTreeView::toggleExpanded(int row)
{
    if (isExpanded(row))
        collapse(row);
    else
        expand(row);
}

bool QQuickTreeView::isModelIndexExpanded(const QModelIndex &modelIndex) const
{
    return isExpanded(mapFromModel(modelIndex).row());
}

int QQuickTreeView::rowAtY(int y, bool includeSpacing)
{
    // (copy from qquicktableview in Qt6)
    Q_D(const QQuickTreeView);

    if (!boundingRect().contains(QPointF(x(), y)))
        return -1;

    const qreal vSpace = d->cellSpacing.height();
    qreal currentRowEnd = d->loadedTableOuterRect.y() - contentY();
    int foundRow = -1;

// in Qt 6.x QQuickTableview uses `QFlatMap` which keys() doesn't allocate
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    for (auto const row : qAsConst(d->loadedRows).keys()) {
#else
    for (auto it = d->loadedRows.keyBegin(), end = d->loadedRows.keyEnd(); it != end; ++it) {
        const auto row = *it;
#endif
        currentRowEnd += d->effectiveRowHeight(row);
        if (y < currentRowEnd) {
            foundRow = row;
            break;
        }
        currentRowEnd += vSpace;
        if (!includeSpacing && y < currentRowEnd) {
            // Hit spacing
            return -1;
        }
        if (includeSpacing && y < currentRowEnd - (vSpace / 2)) {
            foundRow = row;
            break;
        }
    }

    return foundRow;
}

int QQuickTreeView::columnAtX(int x, bool includeSpacing)
{
    // (copy from qquicktableview in Qt6)
    Q_D(const QQuickTreeView);

    if (!boundingRect().contains(QPointF(x, y())))
        return -1;

    const qreal hSpace = d->cellSpacing.width();
    qreal currentColumnEnd = d->loadedTableOuterRect.x() - contentX();
    int foundColumn = -1;

// in Qt 6.x QQuickTableview uses `QFlatMap` which keys() doesn't allocate
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    for (auto const column : qAsConst(d->loadedColumns).keys()) {
#else
    for (auto it = d->loadedColumns.keyBegin(), end = d->loadedColumns.keyEnd(); it != end; ++it) {
        const auto column = *it;
#endif
        currentColumnEnd += d->effectiveColumnWidth(column);
        if (x < currentColumnEnd) {
            foundColumn = column;
            break;
        }
        currentColumnEnd += hSpace;
        if (!includeSpacing && x < currentColumnEnd) {
            // Hit spacing
            return -1;
        } else if (includeSpacing && x < currentColumnEnd - (hSpace / 2)) {
            foundColumn = column;
            break;
        }
    }

    return foundColumn;
}

QModelIndex QQuickTreeView::viewIndex(int column, int row)
{
    return d_func()->m_proxyModel.index(row, column);
}

QModelIndex QQuickTreeView::mapToModel(const QModelIndex &viewIndex) const
{
    return d_func()->m_proxyModel.mapToModel(viewIndex);
}

QModelIndex QQuickTreeView::mapFromModel(const QModelIndex &modelIndex) const
{
    return d_func()->m_proxyModel.mapFromModel(modelIndex);
}

QModelIndex QQuickTreeView::currentIndex() const
{
    return mapFromModel(currentModelIndex());
}

void QQuickTreeView::setCurrentIndex(const QModelIndex &index)
{
    setCurrentModelIndex(mapToModel(index));
}

QModelIndex QQuickTreeView::currentModelIndex() const
{
    return d_func()->m_currentModelIndex;
}

void QQuickTreeView::setCurrentModelIndex(const QModelIndex &modelIndex)
{
    Q_D(QQuickTreeView);
    if (d->m_currentModelIndex == modelIndex)
        return;

    d->m_currentModelIndex = modelIndex;
    d->m_currentModelIndexEmitted = modelIndex;
    d->m_currentItemEmitted = currentItem();

    emit currentIndexChanged();
    emit currentModelIndexChanged();
    emit currentItemChanged();
}

QQuickItem *QQuickTreeView::currentItem() const
{
    return itemAtModelIndex(currentModelIndex());
}

QQuickItem *QQuickTreeView::itemAtCell(const QPoint &cell) const
{
    Q_D(const QQuickTreeView);
    const int modelIndex = d->modelIndexAtCell(cell);
    if (!d->loadedItems.contains(modelIndex))
        return nullptr;
    return d->loadedItems.value(modelIndex)->item;
}

QQuickItem *QQuickTreeView::itemAtIndex(const QModelIndex &viewIndex) const
{
    return itemAtCell(QPoint(viewIndex.column(), viewIndex.row()));
}

QQuickItem *QQuickTreeView::itemAtModelIndex(const QModelIndex &modelIndex) const
{
    return itemAtIndex(mapFromModel(modelIndex));
}

void QQuickTreeView::keyPressEvent(QKeyEvent *e)
{
    Q_D(QQuickTreeView);

    switch (e->key()) {
    case Qt::Key_Up:
        d->moveCurrentViewIndex(0, -1);
        break;
    case Qt::Key_Down:
        d->moveCurrentViewIndex(0, 1);
        break;
    case Qt::Key_Left:
        if (d->m_navigationMode == QQuickTreeView::Table)
            d->moveCurrentViewIndex(-1, 0);
        else
            collapseModelIndex(d->m_currentModelIndex);
        break;
    case Qt::Key_Right:
        if (d->m_navigationMode == QQuickTreeView::Table)
            d->moveCurrentViewIndex(1, 0);
        else
            expandModelIndex(d->m_currentModelIndex);
        break;
    case Qt::Key_Space:
        toggleModelIndexExpanded(d->m_currentModelIndex);
        break;
    default:
        QQuickTableView::keyPressEvent(e);
        break;
    }
}

void QQuickTreeView::mousePressEvent(QMouseEvent *e)
{
    QQuickTableView::mousePressEvent(e);

    d_func()->m_contentItemPosAtMousePress = contentItem()->position();
}

void QQuickTreeView::mouseReleaseEvent(QMouseEvent *e)
{
    QQuickTableView::mouseReleaseEvent(e);

    // Clicking on a tree transfers focus to it
    setFocus(true);

    if (contentItem()->position() != d_func()->m_contentItemPosAtMousePress) {
        // content item was flicked, which should cancel setting current index
        return;
    }

    const int column = columnAtX(e->pos().x(), true);
    const int row = rowAtY(e->pos().y(), true);
    if (column == -1 || row == -1)
        return;

    if (d_func()->m_navigationMode == Table)
        setCurrentModelIndex(mapToModel(viewIndex(column, row)));
    else
        setCurrentModelIndex(mapToModel(viewIndex(0, row)));
}

void QQuickTreeView::mouseDoubleClickEvent(QMouseEvent *e)
{
    QQuickTableView::mouseDoubleClickEvent(e);

    const int row = rowAtY(e->pos().y(), true);
    if (row == -1)
        return;

    toggleExpanded(row);
}

void QQuickTreeView::viewportMoved(Qt::Orientations orientation)
{
    QQuickTableView::viewportMoved(orientation);
    d_func()->checkForPropertyChanges();
}

QQuickTreeViewAttached *QQuickTreeView::qmlAttachedProperties(QObject *obj)
{
    return new QQuickTreeViewAttached(obj);
}

QQuickTreeView::NavigateMode QQuickTreeView::navigationMode() const
{
    return d_func()->m_navigationMode;
}

void QQuickTreeView::setNavigationMode(QQuickTreeView::NavigateMode navigateMode)
{
    Q_D(QQuickTreeView);
    if (d->m_navigationMode == navigateMode)
        return;

    d->m_navigationMode = navigateMode;
    emit navigationModeChanged();
}

QQuickTreeViewStyleHints *QQuickTreeView::styleHints()
{
    return &d_func()->m_styleHints;
}

bool QQuickTreeViewAttached::hasChildren() const
{
    return m_hasChildren;
}

void QQuickTreeViewAttached::setHasChildren(bool hasChildren)
{
    if (m_hasChildren == hasChildren)
        return;

    m_hasChildren = hasChildren;
    emit hasChildrenChanged();
}

bool QQuickTreeViewAttached::isExpanded() const
{
    return m_isExpanded;
}

void QQuickTreeViewAttached::setIsExpanded(bool isExpanded)
{
    if (m_isExpanded == isExpanded)
        return;

    m_isExpanded = isExpanded;
    emit isExpandedChanged();
}

int QQuickTreeViewAttached::depth()
{
    return m_depth;
}

void QQuickTreeViewAttached::setDepth(int depth)
{
    if (m_depth == depth)
        return;

    m_depth = depth;
    emit depthChanged();
}

QT_END_NAMESPACE
