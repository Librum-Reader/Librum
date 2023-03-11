/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
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

import QtQuick 2.15
import Qt.labs.qmlmodels 1.0
import QtQuick.Shapes 1.0
import QtQuick.TreeView 2.15

TreeViewTemplate {
    id: control

    /*
        Note: if you need to tweak this style beyond the styleHints, either
        just assign a custom delegate directly to your TreeView, or copy this file
        into your project and use it as a starting point for your custom version.
    */
    styleHints.indent: 18
    styleHints.columnPadding: 20
    styleHints.foregroundOdd: "black"
    styleHints.backgroundOdd: "transparent"
    styleHints.foregroundEven: "black"
    styleHints.backgroundEven: "transparent"
    styleHints.foregroundCurrent: navigationMode === TreeViewTemplate.List ? "white" : "black"
    styleHints.backgroundCurrent: navigationMode === TreeViewTemplate.List ? "#005fe5" : "transparent"
    styleHints.foregroundHovered: "transparent"
    styleHints.backgroundHovered: "transparent"
    styleHints.overlay:  navigationMode === TreeViewTemplate.Table ? Qt.rgba(0, 0, 0, 0.5) : "transparent"
    styleHints.overlayHovered: "transparent"
    styleHints.indicator: "black"
    styleHints.indicatorHovered: "transparent"

    QtObject {
        // Don't leak API that might not be support
        // by all styles into the public API.
        id: d
        property var hoverIndex: control.viewIndex(-1, -1)
        property bool hoverEnabled: styleHints.foregroundHovered.a > 0 || styleHints.backgroundHovered.a > 0

        function updateHoverIndex(index, hovered) {
            if (hovered)
                hoverIndex = index
            else if (hoverIndex === index)
                hoverIndex = control.viewIndex(-1, -1)
        }

        function bgColor(column, row) {
            if (row === d.hoverIndex.row
                    && (column === d.hoverIndex.column || navigationMode === TreeViewTemplate.List)
                    && styleHints.backgroundHovered.a > 0)
                return styleHints.backgroundHovered
            else if (row === currentIndex.row)
                return styleHints.backgroundCurrent
            else if (row % 2)
                return styleHints.backgroundOdd
            else
                return styleHints.backgroundEven
        }

        function fgColor(column, row) {
            if (row === d.hoverIndex.row
                    && (column === d.hoverIndex.column || navigationMode === TreeViewTemplate.List)
                    && styleHints.foregroundHovered.a > 0)
                return styleHints.foregroundHovered
            else if (row === currentIndex.row)
                return styleHints.foregroundCurrent
            else if (row % 2)
                return styleHints.foregroundOdd
            else
                return styleHints.foregroundEven
        }

        function indicatorColor(column, row) {
            if (row === d.hoverIndex.row
                    && (column === d.hoverIndex.column || navigationMode === TreeViewTemplate.List)
                    && styleHints.indicatorHovered.a > 0)
                return styleHints.indicatorHovered
            else if (row === currentIndex.row)
                return styleHints.indicatorCurrent
            else
                return styleHints.indicator
        }

        function overlayColor() {
            if (d.hoverIndex.row === currentIndex.row
                    && (d.hoverIndex.column === currentIndex.column || navigationMode === TreeViewTemplate.List)
                    && styleHints.overlayHovered.a > 0)
                return styleHints.overlayHovered
            else
                return styleHints.overlay

        }
    }

    delegate: DelegateChooser {
        DelegateChoice {
            // The column where the tree is drawn
            column: 0

            Rectangle {
                id: treeNode
                implicitWidth: treeNodeLabel.x + treeNodeLabel.width + (styleHints.columnPadding / 2)
                implicitHeight: Math.max(indicator.height, treeNodeLabel.height)
                color: d.bgColor(column, row)

                property TreeViewTemplate view: TreeViewTemplate.view
                property bool hasChildren: TreeViewTemplate.hasChildren
                property bool isExpanded: TreeViewTemplate.isExpanded
                property int depth: TreeViewTemplate.depth

                Text {
                    id: indicator
                    x: depth * styleHints.indent
                    color: d.indicatorColor(column, row)
                    font: styleHints.font
                    text: hasChildren ? (isExpanded ? "▼" : "▶") : ""
                    anchors.verticalCenter: parent.verticalCenter

                    TapHandler {
                        onTapped: {
                            if (hasChildren)
                                control.toggleExpanded(row)
                        }
                    }
                }

                Text {
                    id: treeNodeLabel
                    x: indicator.x + Math.max(styleHints.indent, indicator.width * 1.5)
                    clip: true
                    color: d.fgColor(column, row)
                    font: styleHints.font
                    text: model.display
                }

                HoverHandler {
                    enabled: d.hoverEnabled
                    onHoveredChanged: d.updateHoverIndex(control.viewIndex(column, row), hovered)
                }
            }
        }

        DelegateChoice {
            //  The remaining columns after the tree column will use this delegate
            Rectangle {
                implicitWidth: infoLabel.x + infoLabel.width + (styleHints.columnPadding / 2)
                color: d.bgColor(column, row)
                Text {
                    id: infoLabel
                    x: styleHints.columnPadding / 2
                    color: d.fgColor(column, row)
                    font: styleHints.font
                    text: display
                    clip: true
                }

                HoverHandler {
                    enabled: d.hoverEnabled
                    onHoveredChanged: d.updateHoverIndex(control.viewIndex(column, row), hovered)
                }
            }
        }
    }

    Shape {
        id: overlayShape
        z: 10
        property point currentPos: currentItem ? mapToItem(overlayShape, Qt.point(currentItem.x, currentItem.y)) : Qt.point(0, 0)
        visible: currentItem != null

        ShapePath {
            id: path
            fillColor: "transparent"
            strokeColor: d.overlayColor()
            strokeWidth: 1
            strokeStyle: ShapePath.DashLine
            dashPattern: [1, 2]
            startX: currentItem ? currentItem.x + strokeWidth : 0
            startY: currentItem ? currentItem.y + strokeWidth : 0
            property real endX: currentItem ? currentItem.width + startX - (strokeWidth * 2) : 0
            property real endY: currentItem ? currentItem.height + startY - (strokeWidth * 2) : 0
            PathLine { x: path.endX; y: path.startY }
            PathLine { x: path.endX; y: path.endY }
            PathLine { x: path.startX; y: path.endY }
            PathLine { x: path.startX; y: path.startY }
        }
    }

}
