/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtPDF module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QQUICKTABLEVIEWEXTRA_P_H
#define QQUICKTABLEVIEWEXTRA_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtQml/qqml.h>
#include <QtQuick/qquickitem.h>
#include <QPointF>
#include <QPolygonF>
#include <QVariant>

QT_BEGIN_NAMESPACE

class QQuickTableView;

class QQuickTableViewExtra : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickTableView* tableView READ tableView WRITE setTableView)

public:
    QQuickTableViewExtra(QObject* parent = nullptr);

    QQuickTableView* tableView() const
    {
        return m_tableView;
    }

    void setTableView(QQuickTableView* tableView)
    {
        m_tableView = tableView;
    }

    Q_INVOKABLE QPoint cellAtPos(qreal x, qreal y) const;

    Q_INVOKABLE QQuickItem* itemAtCell(int column, int row) const
    {
        return itemAtCell(QPoint(column, row));
    }

    Q_INVOKABLE QQuickItem* itemAtCell(const QPoint& cell) const;

    Q_INVOKABLE void positionViewAtCell(int column, int row,
                                        Qt::Alignment alignment,
                                        const QPointF& offset = QPointF())
    {
        positionViewAtCell(QPoint(column, row), alignment, offset);
    }

    Q_INVOKABLE void positionViewAtCell(const QPoint& cell,
                                        Qt::Alignment alignment,
                                        const QPointF& offset);

    Q_INVOKABLE void positionViewAtRow(int row, Qt::Alignment alignment,
                                       qreal offset = 0)
    {
        positionViewAtCell(QPoint(0, row), alignment & Qt::AlignVertical_Mask,
                           QPointF(0, offset));
    }

private:
    QQuickTableView* m_tableView = nullptr;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QQuickTableViewExtra)

#endif  // QQUICKTABLEVIEWEXTRA_P_H