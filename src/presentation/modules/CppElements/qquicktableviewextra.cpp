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

#include <QtQuick/private/qquicktableview_p.h>
#include <QQmlContext>
#include <QtQml>
#include "qquicktableviewextra_p.h"

Q_LOGGING_CATEGORY(qLcTVE, "qt.pdf.tableextra")

QT_BEGIN_NAMESPACE

/*!
    \internal
    \qmltype TableViewExtra
    \instantiates QQuickTableViewExtra
    \inqmlmodule QtQuick.Pdf
    \ingroup pdf
    \brief A helper class with missing TableView functions
    \since 5.15
    TableViewExtra provides equivalents for some functions that will be added
    to TableView in Qt 6.
*/

QQuickTableViewExtra::QQuickTableViewExtra(QObject* parent) :
    QObject(parent)
{
}

QPoint QQuickTableViewExtra::cellAtPos(qreal x, qreal y) const
{
    QPointF position(x, y);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return m_tableView->cellAtPos(position);
#else
    if(!m_tableView->boundingRect().contains(position))
        return QPoint(-1, -1);

    const QQuickItem* contentItem = m_tableView->contentItem();

    for(const QQuickItem* child : contentItem->childItems())
    {
        const QPointF posInChild = m_tableView->mapToItem(child, position);
        if(child->boundingRect().contains(posInChild))
        {
            const auto context = qmlContext(child);
            const int column = context->contextProperty("column").toInt();
            const int row = context->contextProperty("row").toInt();
            return QPoint(column, row);
        }
    }

    return QPoint(-1, -1);
#endif
}

QQuickItem* QQuickTableViewExtra::itemAtCell(const QPoint& cell) const
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return m_tableView->itemAtCell(cell);
#else
    const QQuickItem* contentItem = m_tableView->contentItem();
    auto children = contentItem->childItems();

    for(QQuickItem* child : children)
    {
        const auto context = qmlContext(child);
        const int column = context->contextProperty("column").toInt();
        const int row = context->contextProperty("row").toInt();
        if(QPoint(column, row) == cell)
            return child;
    }

    return nullptr;
#endif
}

void QQuickTableViewExtra::positionViewAtCell(const QPoint& cell,
                                              Qt::Alignment alignment,
                                              const QPointF& offset)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    m_tableView->positionViewAtCell(cell, alignment, offset);
#else
    // Note: this fallback implementation assumes all cells to be of the same
    // size!

    if(cell.x() < 0 || cell.x() > m_tableView->columns() - 1)
        return;
    if(cell.y() < 0 || cell.y() > m_tableView->rows() - 1)
        return;

    Qt::Alignment verticalAlignment =
        alignment & (Qt::AlignTop | Qt::AlignVCenter | Qt::AlignBottom);
    Qt::Alignment horizontalAlignment =
        alignment & (Qt::AlignLeft | Qt::AlignHCenter | Qt::AlignRight);

    const QQuickItem* contentItem = m_tableView->contentItem();
    const QQuickItem* randomChild = contentItem->childItems().first();
    const qreal cellWidth = randomChild->width();
    const qreal cellHeight = randomChild->height();

    if(!verticalAlignment && !horizontalAlignment)
    {
        qmlWarning(this) << "No valid alignment specified";
        return;
    }

    if(horizontalAlignment)
    {
        qreal newPosX = 0;
        const qreal columnPosLeft =
            int(cell.x() * (cellWidth + m_tableView->columnSpacing()));
        m_tableView->setContentX(0);
        m_tableView->forceLayout();
        m_tableView->setContentX(columnPosLeft);
        m_tableView->forceLayout();

        switch(horizontalAlignment)
        {
        case Qt::AlignLeft:
            newPosX = m_tableView->contentX() + offset.x();
            break;
        case Qt::AlignHCenter:
            newPosX = m_tableView->contentX() - m_tableView->width() / 2 +
                      (cellWidth / 2) + offset.x();
            break;
        case Qt::AlignRight:
            newPosX = m_tableView->contentX() - m_tableView->width() +
                      cellWidth + offset.x();
            break;
        }

        m_tableView->setContentX(newPosX);
        m_tableView->forceLayout();
    }

    if(verticalAlignment)
    {
        qreal newPosY = 0;
        const qreal rowPosTop =
            int(cell.y() * (cellHeight + m_tableView->rowSpacing()));
        m_tableView->setContentY(0);
        m_tableView->forceLayout();
        m_tableView->setContentY(rowPosTop);
        m_tableView->forceLayout();

        switch(verticalAlignment)
        {
        case Qt::AlignTop:
            newPosY = m_tableView->contentY() + offset.y();
            break;
        case Qt::AlignVCenter:
            newPosY = m_tableView->contentY() - m_tableView->height() / 2 +
                      (cellHeight / 2) + offset.y();
            break;
        case Qt::AlignBottom:
            newPosY = m_tableView->contentY() - m_tableView->height() +
                      cellHeight + offset.y();
            break;
        }

        m_tableView->setContentY(newPosY);
        m_tableView->forceLayout();
    }
#endif
}

QT_END_NAMESPACE