
# TreeView

**import:** QtQuick.TreeView

**Inherits:** TableView

Provides  a  tree  view to  display  data  from  a  QAbstractItemModel.

## Build
- git clone https://code.qt.io/qt-extensions/qttreeview.git
- cd treeview
- qmake && make && make install
- make sub-examples (if you want the examples built as well)

**NOTE:** If you use Qt 6, you can also build with cmake. But if you're using a Qt version
below Qt 6.2, you need to do "git checkout 6.1" before building.

## Documentation

#### Important changes as of Qt 6.3
As of Qt 6.3, a TreeView is also available as an integral part of Qt Quick. That
version is based on the implementation found in this repository, but the API has been
refined and will in some cases differ. The biggest change is that the styling API
is moved out of TreeView and into the delegate. This is done in collaboration with
Qt Quick Controls, which offers a new customizable control, TreeViewDelegate, that
renders the tree using the application style.

This means that if you use the TreeView in this module with Qt 6.3 (or above), you
should import it into a namespace to avoid any name clashes with the version in Qt Quick.
E.g "import QtQuick.TreeView as QtMarketplace" and then do "QtMarketplace.TreeView { ... }"

If possible, we recommend that you use the TreeView in Qt Quick if you can,
since any new features will most likely only go into that version.

**note**: A short-coming with the TreeView in Qt Quick (Qt 6.3) compared to the version
in this repository, is that it has no support for manipulating currentIndex (that is,
selecting and moving the current index using the mouse or the keyboard). But this is
expected to be available in later versions of Qt.

#### int  currentIndex
This property holds the view index that has been selected as current. Selecting an index to be current is done by either using the arrow keys, clicking on a row (or a cell) with the mouse, or assign *currentIndex* a value explicitly.

**Note**: *currentIndex* is an index into the the view model. In this model, all tree nodes that are *visible* in the view are flattened into a list that can be shown in a table column. This also means that *currentIndex.row* is the table row relative to root node. To get the current index in the model, where the row is relative to parent node, use *currentModelIndex* or  *mapToModel()*:

	var indexInModel = currentModelIndex
	var indexInModelMapped = mapToModel(currentIndex)
	console.assert(indexInModel == indexInModelMapped)

If no index has been selected as current, or if *currentModelIndex* is is not visible in the view, *currentIndex.valid* will be *false*.

**See also:** currentModelIndex,  currentItem,  mapToModel()

####  int currentModelIndex
This property holds the model index that has been selected as current. Selecting
an index to be current is done by either using the arrow keys, clicking on a row (or a cell) with the mouse, or assign *currentModelIndex* a value explicitly.

**Note:** *currentModelIndex* is an index into the the model. This also means
that *currentModelIndex.row* is the child number relative to the parent node.
To get the current row in the view, which is relative to the root, use *currentIndex* or *mapFromModel()*:

    var indexInView = currentViewIndex
    var indexInViewMapped = mapFromModel(currentModelIndex)
    console.assert(indexInView == indexInViewMapped)

If no index has been selected as current, *currentModelIndex.valid* will be *false*.

**See also:** currentIndex, currentItem, mapFromModel()

####  readonly Item currentItem
This property holds the delegate item that is selected as current. Selecting an item to be current is done by either using the arrow keys, clicking on a row (or a cell) with the mouse, or assign a value to *currentIndex* explicitly.

**Note:** Using *currentItem* is *almost* the same as calling itemAtModelIndex(currentModelIndex). The difference is that the latter expression will only change when *currentModelIndex* change. But *currentItem* can also change as a result of the view doing a relayout behind the scenes (which can force delegate items to be reloaded or recycled). Since the item can be recycled or destroyed at any point in time behind the applications back, you should never store this value.

if *currentIndex.valid* is *false*, this property will be *null*.

**See also:** itemAtCell(), currentIndex, currentModelIndex()

#### int navigationMode
This property holds how you would like the user to be able
navigate the tree. It can have the following values:

- TreeView.List - the tree view acts like as a list view. This means that
    the user can only navigate up and down in the tree, but not sideways to
    the other columns. In this mode the left and right arrow keys will collapse
    and expand nodes in the tree (in addition to *Qt.Key_Space*).
- TreeView.Table - the tree view acts like a table view. This means that
    the user can navigate up and down, but also left and right through
    all the columns in the tree. In this mode the left and right arrow keys will
    move *currentIndex* left and right, and only *Qt.Key_Space* will toggle
    nodes to be collapsed or expanded.

**Note:** the selected mode can also affect how the tree is styled by the delegate.

#### int styleHints
This property holds a set of hints that can be used to tweak the
appearance of the *delegate* without the need to create a custom one.
The hints that can be set is the following:

- styleHints.indicator - the color of the collapsed/expanded icon
- styleHints.indicatorCurrent - the color of the current row's collapsed/expanded icon
- styleHints.indicatorHovered - the color of the collapsed/expanded icon when hovered
- styleHints.overlay - the color of the overlay used for *currentIndex*
- styleHints.overlayHovered - the color of the overlay used for *currentIndex* when hovered
- styleHints.foregroundOdd - the foreground color of odd rows
- styleHints.backgroundOdd - the background color of odd rows
- styleHints.foregroundEven - the foreground color of even rows
- styleHints.backgroundEven - the background color of even rows
- styleHints.foregroundCurrent - the foreground color of the current item (or
    the whole row, if *selectionMode* is *TreeView.List*)
- styleHints.backgroundCurrent - the background color of the current item (or
    the whole row, if *selectionMode* is *TreeView.List*)
- styleHints.foregroundHovered - the foreground color of the current item (or
    the whole row, if *selectionMode* is *TreeView.List*) when hovered
- styleHints.backgroundHovered - the background color of the current item when hovered (or
    the whole row, if *selectionMode* is *TreeView.List*)
- styleHints.indent - the horizontal space between a parent and a child node
- styleHints.columnPadding - the padding between a cell and its contents
- styleHints.font - the font used by text items in the delegate

**Note:** a delegate is free to ignore any hints specified by the application. If you need
to style a TreeView beyond what the hints can offer, you can always assign your own *delegate*. Taking
a copy of the TreeView.qml file that is included in the repository can be a good starting point for
doing so.

#### bool hasChildren(row)
Returns if the given *row* in the view is shown as expanded.

**Note:** *row* should be the row in the view, not in the model.

**See also:** viewIndex

#### bool hasSiblings(row)
Returns if the given *row* in the view has siblings.

**Note:** *row* should be the row in the view, not in the model.

**See also:** viewIndex

#### int depth(row)
Returns the depth (the number of parents up to
the root) of the given *row*.

**Note:** *row* should be the row in the view, not in the model.

**See also:** viewIndex

#### bool isExpanded(row)
Returns if the given *row* in the view is shown as expanded.

**Note:** *row* should be the row in the view, not in the model.

**See also:** viewIndex

#### expand(row)
Expands the tree node at the given *row* in the view.

**Note:** this function will not affect the model, only
the visual representation in the view.

**See also:** viewIndex, collapse(), expandModelIndex(), collapseModelIndex(), isCollapsed()

#### collapse(row)
Collapses the tree node at the given *row* in the view.

**Note:** this function will not affect the model, only
the visual representation in the view.

**See also:** viewIndex, expand(), expandModelIndex(), collapseModelIndex(), isCollapsed()

#### toggleExpanded(row)
Toggles if the tree node at the given *row* should be expanded. This is a convenience for doing:

	if (isExpanded(row))
	    collapse(row)
	else
	    expand(row)

#### bool isModelIndexExpanded(modelIndex)
Returns if the given *modelIndex* is shown as expanded in the view. This is a convenience for doing:

	isExpanded(mapFromModel(modelIndex).row)

**See also:** viewIndex

#### collapseModelIndex(modelIndex)
Collapses the tree node at the given *modelIndex*. This is a convenience for doing:

	collapse(mapFromModel(modelIndex).row)

**See also:** viewIndex, collapse(), expand(), expandModelIndex(), isCollapsed()

#### expandModelIndex(modelIndex)
Expands the tree node at the given *modelIndex*. This is a convenience for doing:

	expand(mapFromModel(modelIndex).row)

**See also:** viewIndex, collapse(), expand(), collapseModelIndex(), isCollapsed()

#### toggleModelIndexExpanded(modelIndex)
Toggles if the tree node at the given *modelIndex* should be expanded. This is a convenience for doing:

	if (isModelIndexExpanded(modelIndex))
	    collapseModelIndex(modelIndex)
	else
	    expandModelIndex(modelIndex)

#### int columnAtX(x, includeSpacing)
Returns the column under *x* in view coordinates.

If *includeSpacing* is set to *false*, and *x* is on top
of the spacing between the columns, the return value will be *-1*.
Otherwise, if *x* is on top of the spacing and *includeSpacing*
is set to *true*, the column closest to the position will be returned.

**See also:** columnSpacing

#### int rowAtY(y, includeSpacing)
Returns the row under *y* in view coordinates.

If *includeSpacing* is set to *false*, and *y* is on top
of the spacing between the rows, the return value will be *-1*.
Otherwise, if *y* is on top of the spacing and *includeSpacing*
is set to *true*, the row closest to the position will be returned.

*See also:** rowSpacing

#### Item itemAtCell(point cell)
Returns the delegate item inside the given table cell. If the cell is not visible in the view,  *null* will be returned.

**See also:** currentItem, itemAtIndex()

#### Item itemAtIndex(viewIndex)
Returns the delegate item at the given view index. If *viewIndex.valid*
is *false*, *null* will be returned.

**See also:** currentItem, itemAtCell(), viewIndex(), itemAtModelIndex()

#### Item itemAtModelIndex(modelIndex)
Returns the delegate item at the given model index. If the item that
represents the model index is not visible in the view, *null*
will be returned. Convenience for doing:

	itemAtIndex(mapFromModel(modelIndex))

**See also:** currentItem, itemAtCell(), viewIndex(), itemAtIndex()

#### QModelIndex viewIndex(column, row)
Creates a model index into the view model from the given *row* and *column*.

**Note:** this index is only valid for use with the view. To create
a model index to use with the model, refer to the *model* API.

#### QModelIndex mapToModel(viewIndex)
Maps an index pointing to an item in the view (the view model), to the
corresponding model index in the *model*.

TreeView uses a view model internally to convert a tree model into
a model suited to be shown in a *TableView*. This view model will
flatten the parts of the tree model that at any point in time is visible
inside view, to a list. Depending on what you need to do, it's sometimes
easier to work with model indexes rather than view indexes, and vice-versa.

**See also:** mapFromModel()

#### QModelIndex mapFromModel(modelIndex)
Maps an index pointing to an item in the \l model to the corresponding model index in the view (the view model).

TreeView uses a view model internally to convert a tree model into
a model suited to be shown in a *TableView*. This view model will
flatten the parts of the tree model that at any point in time is
visible inside view, to a list. Depending on what you need to
do, it's sometimes easier to work with model indexes rather than
view indexes, and vice-versa.

**See also:** mapToModel()

### Attached properties
#### TreeView TreeView.view
This attached property holds the view that manages the delegate instance. It is attached to each instance of the delegate.

#### bool TreeView.hasChildren
This attached property holds if the delegate instance is to be shown as
having children (the model index represented by the instance has children
in the model). It is attached to each instance of the delegate.

#### bool TreeView.isExpanded
This attached property holds if the delegate instance is to be shown as
expanded (the view index represented by the instance is expanded in the view model). It is attached to each instance of the delegate.

#### int TreeView.depth
This attached property holds the tree depth (the number of parents up to the root) of the delegate instance. It is attached to each instance of the delegate.





