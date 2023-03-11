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

#ifndef QQUICKTREEVIEW_P_P_H
#define QQUICKTREEVIEW_P_P_H

#include <QtQuick/private/qquicktableview_p_p.h>
#include "qquicktreemodeladaptor_p.h"
#include "qquicktreeview_p.h"

QT_BEGIN_NAMESPACE

class QQuickTreeViewPrivate : public QQuickTableViewPrivate
{
public:
    Q_DECLARE_PUBLIC(QQuickTreeView)
    QQuickTreeViewPrivate();
    ~QQuickTreeViewPrivate() override;

    QVariant modelImpl() const override;
    void setModelImpl(const QVariant &newModel) override;
    void syncModel() override;

    QQuickItem *itemAtCell(const QPoint &cell) const;
    qreal effectiveRowHeight(int row) const;
    qreal effectiveColumnWidth(int column) const;

    void moveCurrentViewIndex(int directionX, int directionY);
    QQuickTreeViewAttached *getAttachedObject(const QObject *object) const;

    void dataChangedCallback(const QModelIndex &topLeft,
                             const QModelIndex &bottomRight, const QVector<int> &roles);

#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
    void initItemCallback(int modelIndex, QObject *object);
    void itemReusedCallback(int modelIndex, QObject *object);
#else
    void initItemCallback(int modelIndex, QObject *object) override;
    void itemReusedCallback(int modelIndex, QObject *object) override;
#endif

    void updateAttachedProperties(QObject *object);

    void checkForPropertyChanges();
    void updatePolish() override;

public:
    // QQuickTreeModelAdaptor1 basically takes a tree model and flattens
    // it into a list (which will be displayed in the first column of
    // the table). Each node in the tree can have several columns of
    // data in the model, which we show in the remaining columns of the table.
    QQuickTreeModelAdaptor m_proxyModel;
    QVariant m_assignedModel;
    QPersistentModelIndex m_currentModelIndex;
    QModelIndex m_currentModelIndexEmitted;
    QQuickItem *m_currentItemEmitted = nullptr;
    QQuickTreeView::NavigateMode m_navigationMode = QQuickTreeView::List;
    QPointF m_contentItemPosAtMousePress;
    QQuickTreeViewStyleHints m_styleHints;
};

QT_END_NAMESPACE

#endif // QQUICKTREEVIEW_P_P_H
