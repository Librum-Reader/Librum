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

#ifndef QQUICKTREEVIEW_P_H
#define QQUICKTREEVIEW_P_H

#include <QtCore/qabstractitemmodel.h>
#include <QtQuick/private/qquicktableview_p.h>

QT_BEGIN_NAMESPACE

class QQuickTreeViewPrivate;
class QQuickTreeViewAttached;

class QQuickTreeViewStyleHints : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QColor indicator MEMBER m_indicator NOTIFY indicatorChanged);
    Q_PROPERTY(QColor indicatorCurrent MEMBER m_indicatorCurrent NOTIFY indicatorCurrentChanged);
    Q_PROPERTY(QColor indicatorHovered MEMBER m_indicatorHovered NOTIFY indicatorHoveredChanged);
    Q_PROPERTY(QColor overlay MEMBER m_overlay NOTIFY overlayChanged);
    Q_PROPERTY(QColor overlayHovered MEMBER m_overlayHovered NOTIFY overlayHoveredChanged);
    Q_PROPERTY(QColor foregroundOdd MEMBER m_foregroundOdd NOTIFY foregroundOddChanged);
    Q_PROPERTY(QColor backgroundOdd MEMBER m_backgroundOdd NOTIFY backgroundOddChanged);
    Q_PROPERTY(QColor foregroundEven MEMBER m_foregroundEven NOTIFY foregroundEvenChanged);
    Q_PROPERTY(QColor backgroundEven MEMBER m_backgroundEven NOTIFY backgroundEvenChanged);
    Q_PROPERTY(QColor foregroundCurrent MEMBER m_foregroundCurrent NOTIFY foregroundCurrentChanged);
    Q_PROPERTY(QColor backgroundCurrent MEMBER m_backgroundCurrent NOTIFY backgroundCurrentChanged);
    Q_PROPERTY(QColor foregroundHovered MEMBER m_foregroundHovered NOTIFY foregroundHoveredChanged);
    Q_PROPERTY(QColor backgroundHovered MEMBER m_backgroundHovered NOTIFY backgroundHoveredChanged);
    Q_PROPERTY(qreal indent MEMBER m_indent NOTIFY indentChanged);
    Q_PROPERTY(qreal columnPadding MEMBER m_columnPadding NOTIFY columnPaddingChanged);
    Q_PROPERTY(QFont font MEMBER m_font NOTIFY fontChanged);

    QML_NAMED_ELEMENT(TreeViewStyleHints)
    QML_UNCREATABLE("TreeViewStyleHints is not creatable")
#ifdef QML_ADDED_IN_VERSION
    QML_ADDED_IN_VERSION(2, 15)
#else
    QML_ADDED_IN_MINOR_VERSION(15)
#endif

signals:
    void indicatorChanged();
    void indicatorCurrentChanged();
    void indicatorHoveredChanged();
    void overlayChanged();
    void overlayHoveredChanged();
    void foregroundOddChanged();
    void backgroundOddChanged();
    void foregroundEvenChanged();
    void backgroundEvenChanged();
    void foregroundCurrentChanged();
    void backgroundCurrentChanged();
    void foregroundHoveredChanged();
    void backgroundHoveredChanged();
    void indentChanged();
    void columnPaddingChanged();
    void fontChanged();

private:
    QColor m_indicator;
    QColor m_indicatorCurrent;
    QColor m_indicatorHovered;
    QColor m_overlay;
    QColor m_overlayHovered;
    QColor m_foregroundOdd;
    QColor m_backgroundOdd;
    QColor m_foregroundEven;
    QColor m_backgroundEven;
    QColor m_foregroundCurrent;
    QColor m_backgroundCurrent;
    QColor m_foregroundHovered;
    QColor m_backgroundHovered;
    qreal m_indent = 0;
    qreal m_columnPadding = 0;
    QFont m_font;
};

// This struct in necessary in order for TreeView to provide
// access to revisioned properties in QQuickTableView
struct QQuickTableViewForeign
{
    Q_GADGET
    QML_ANONYMOUS
    QML_FOREIGN(QQuickTableView)
#ifdef QML_ADDED_IN_VERSION
    QML_ADDED_IN_VERSION(2, 12)
#else
    QML_ADDED_IN_MINOR_VERSION(12)
#endif
};

class QQuickTreeView : public QQuickTableView
{
    Q_OBJECT

    Q_PROPERTY(QModelIndex currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged);
    Q_PROPERTY(QModelIndex currentModelIndex READ currentModelIndex WRITE setCurrentModelIndex NOTIFY currentModelIndexChanged);
    Q_PROPERTY(QQuickItem *currentItem READ currentItem NOTIFY currentItemChanged);
    Q_PROPERTY(NavigateMode navigationMode READ navigationMode WRITE setNavigationMode NOTIFY navigationModeChanged);
    Q_PROPERTY(QQuickTreeViewStyleHints *styleHints READ styleHints CONSTANT);

    QML_NAMED_ELEMENT(TreeViewTemplate)
#ifdef QML_ADDED_IN_VERSION
    QML_ADDED_IN_VERSION(2, 15)
#else
    QML_ADDED_IN_MINOR_VERSION(15)
#endif
    QML_ATTACHED(QQuickTreeViewAttached)

public:
    enum NavigateMode { List, Table };
    Q_ENUM(NavigateMode)

    QQuickTreeView(QQuickItem *parent = nullptr);
    ~QQuickTreeView() override;

    Q_INVOKABLE bool hasChildren(int row) const;
    Q_INVOKABLE bool hasSiblings(int row) const;
    Q_INVOKABLE int depth(int row) const;

    Q_INVOKABLE bool isExpanded(int row) const;
    Q_INVOKABLE void expand(int row);
    Q_INVOKABLE void collapse(int row);
    Q_INVOKABLE void toggleExpanded(int row);

    Q_INVOKABLE bool isModelIndexExpanded(const QModelIndex &modelIndex) const;
    Q_INVOKABLE void collapseModelIndex(const QModelIndex &modelIndex);
    Q_INVOKABLE void expandModelIndex(const QModelIndex &modelIndex);
    Q_INVOKABLE void toggleModelIndexExpanded(const QModelIndex &modelIndex);

    Q_INVOKABLE int columnAtX(int x, bool includeSpacing);
    Q_INVOKABLE int rowAtY(int y, bool includeSpacing);

    Q_INVOKABLE QQuickItem *itemAtCell(const QPoint &cell) const;
    Q_INVOKABLE QQuickItem *itemAtIndex(const QModelIndex &viewIndex) const;
    Q_INVOKABLE QQuickItem *itemAtModelIndex(const QModelIndex &modelIndex) const;

    Q_INVOKABLE QModelIndex viewIndex(int column, int row);
    Q_INVOKABLE QModelIndex mapToModel(const QModelIndex &viewIndex) const;
    Q_INVOKABLE QModelIndex mapFromModel(const QModelIndex &modelIndex) const;

    QModelIndex currentIndex() const;
    void setCurrentIndex(const QModelIndex &index);
    QQuickItem *currentItem() const;

    QModelIndex currentModelIndex() const;
    void setCurrentModelIndex(const QModelIndex &modelIndex);

    NavigateMode navigationMode() const;
    void setNavigationMode(NavigateMode navigateMode);

    QQuickTreeViewStyleHints *styleHints();

    static QQuickTreeViewAttached *qmlAttachedProperties(QObject *obj);

protected:
    void viewportMoved(Qt::Orientations orientation) override;
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;

signals:
    void currentIndexChanged();
    void currentModelIndexChanged();
    void expanded(int row);
    void collapsed(int row);
    void navigationModeChanged();
    void currentItemChanged();

private:
    Q_DISABLE_COPY(QQuickTreeView)
    Q_DECLARE_PRIVATE(QQuickTreeView)
};

class QQuickTreeViewAttached : public QQuickTableViewAttached
{
    Q_OBJECT
    Q_PROPERTY(QQuickTreeView *view READ view NOTIFY viewChanged)
    Q_PROPERTY(bool hasChildren READ hasChildren NOTIFY hasChildrenChanged)
    Q_PROPERTY(bool isExpanded READ isExpanded NOTIFY isExpandedChanged)
    Q_PROPERTY(int depth READ depth NOTIFY depthChanged)

public:
    QQuickTreeViewAttached(QObject *parent) : QQuickTableViewAttached(parent) {}
    QQuickTreeView *view() const { return m_view; }
    void setView(QQuickTreeView *view) { m_view = view; };

    bool hasChildren() const;
    void setHasChildren(bool hasChildren);

    bool isExpanded() const;
    void setIsExpanded(bool isExpanded);

    int depth();
    void setDepth(int depth);

Q_SIGNALS:
    void viewChanged();
    void hasChildrenChanged();
    void isExpandedChanged();
    void depthChanged();

private:
    QPointer<QQuickTreeView> m_view;
    bool m_hasChildren = false;
    bool m_isExpanded = false;
    int m_depth = -1;

    friend class QQuickTreeViewPrivate;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QQuickTreeView)

#endif // QQUICKTREEVIEW_P_H
