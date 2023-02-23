import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.elements 1.0
import Librum.models 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Qt.labs.qmlmodels 1.0
import QtQuick.Shapes 1.0
import QtQuick.TreeView 2.15 as Chapters


Item
{
    id: root
    property var chapterModel
    
    implicitWidth: 300
    implicitHeight: 600
    
    Rectangle
    {
        anchors.fill: parent
        color: "transparent"
        
        
        ColumnLayout
        {
            anchors.fill: parent
            spacing: 0
            
            
            Label
            {
                id: header
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 26
                text: "Contents"
                font.pointSize: 14
                font.weight: Font.Medium
                color: Style.colorText
            }
            
            Rectangle
            {
                id: searchBox
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                Layout.margins: 12
                Layout.alignment: Qt.AlignBottom
                radius: 4
                color: Style.colorContainerBackground
                border.color: Style.colorButtonBorder
                
                TextField
                {
                    id: inputField
                    anchors.fill: parent
                    leftPadding: 15
                    rightPadding: 15
                    selectByMouse: true
                    color: Style.colorText
                    font.pointSize: 11
                    font.weight: Font.Normal
                    placeholderText: "Search..."
                    placeholderTextColor: Style.colorPlaceholderText
                    background: Rectangle   
                    {   
                        anchors.fill: parent
                        radius: 4
                        color: "transparent"
                    }
                }
            }
            
            Pane
            {
                id: treeViewContainer
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 12
                Layout.topMargin: 0
                verticalPadding: 6
                horizontalPadding: 4
                background: Rectangle
                {
                    radius: 4
                    color: Style.colorContainerBackground
                    border.color: Style.colorContainerBorder
                }
                
                
                ScrollView
                {
                    anchors.fill: parent
                    
                    
                    Chapters.TreeView
                    {
                        id: treeView
                        anchors.fill: parent
                        anchors.margins: 1
                        model: root.chapterModel
                        clip: true
                        focus: true
                        navigationMode: Chapters.TreeView.List
                        
                        styleHints.indent: 18
                        styleHints.columnPadding: 20
                        styleHints.foregroundOdd: "black"
                        styleHints.backgroundOdd: "transparent"
                        styleHints.foregroundEven: "black"
                        styleHints.backgroundEven: "transparent"
                        styleHints.foregroundCurrent: navigationMode === treeView.List ? "white" : "black"
                        styleHints.backgroundCurrent: navigationMode === treeView.List ? "#005fe5" : "transparent"
                        styleHints.foregroundHovered: "transparent"
                        styleHints.backgroundHovered: "transparent"
                        styleHints.overlay:  navigationMode === treeView.Table ? Qt.rgba(0, 0, 0, 0.5) : "transparent"
                        styleHints.overlayHovered: "transparent"
                        styleHints.indicator: "black"
                        styleHints.indicatorHovered: "transparent"
                        
                        delegate:  DelegateChooser
                        {
                            DelegateChoice
                            {
                                // The column where the tree is drawn
                                column: 0
                                
                                Rectangle
                                {
                                    id: treeNode
                                    implicitWidth: treeNodeLabel.x + treeNodeLabel.width + (treeView.styleHints.columnPadding / 2)
                                    implicitHeight: Math.max(indicator.height, treeNodeLabel.height)
                                    color: d.bgColor(column, row)
                                    
                                    property var view: Chapters.TreeView.view
                                    property bool hasChildren: Chapters.TreeView.hasChildren
                                    property bool isExpanded: Chapters.TreeView.isExpanded
                                    property int depth: Chapters.TreeView.depth
                                    
                                    Text
                                    {
                                        id: indicator
                                        x: depth * treeView.styleHints.indent
                                        color: d.indicatorColor(column, row)
                                        font: treeView.styleHints.font
                                        text: hasChildren ? (isExpanded ? "▼" : "▶") : ""
                                        anchors.verticalCenter: parent.verticalCenter
                                        
                                        TapHandler
                                        {
                                            onTapped:
                                            {
                                                if (hasChildren)
                                                    treeView.toggleExpanded(row)
                                            }
                                        }
                                    }
                                    
                                    Text
                                    {
                                        id: treeNodeLabel
                                        x: indicator.x + Math.max(treeView.styleHints.indent, indicator.width * 1.5)
                                        clip: true
                                        color: d.fgColor(column, row)
                                        font: treeView.styleHints.font
                                        text: model.display
                                    }
                                    
                                    HoverHandler
                                    {
                                        enabled: d.hoverEnabled
                                        onHoveredChanged: d.updateHoverIndex(treeView.viewIndex(column, row), hovered)
                                    }
                                }
                            }
                            
                            DelegateChoice
                            {
                                // The remaining columns after the tree column will use this delegate
                                Rectangle
                                {
                                    implicitWidth: infoLabel.x + infoLabel.width + (treeView.styleHints.columnPadding / 2)
                                    color: d.bgColor(column, row)
                                    Text
                                    {
                                        id: infoLabel
                                        x: treeView.styleHints.columnPadding / 2
                                        color: d.fgColor(column, row)
                                        font: treeView.styleHints.font
                                        text: display
                                        clip: true
                                    }
                                    
                                    HoverHandler
                                    {
                                        enabled: d.hoverEnabled
                                        onHoveredChanged: d.updateHoverIndex(treeView.viewIndex(column, row), hovered)
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
                }
            }
        }
    }
    
    // Don't leak API that might not be support by all styles into the public API.
    QtObject
    {
        id: d
        property var hoverIndex: treeView.viewIndex(-1, -1)
        property bool hoverEnabled: treeView.styleHints.foregroundHovered.a > 0 || treeView.styleHints.backgroundHovered.a > 0
        
        function updateHoverIndex(index, hovered)
        {
            if (hovered)
                hoverIndex = index
            else if (hoverIndex === index)
                hoverIndex = treeView.viewIndex(-1, -1)
        }
        
        function bgColor(column, row)
        {
            if (row === d.hoverIndex.row
                    && (column === d.hoverIndex.column || navigationMode === treeView.List)
                    && treeView.styleHints.backgroundHovered.a > 0)
                return treeView.styleHints.backgroundHovered
            else if (row === treeView.currentIndex.row)
                return treeView.styleHints.backgroundCurrent
            else if (row % 2)
                return treeView.styleHints.backgroundOdd
            else
                return treeView.styleHints.backgroundEven
        }
        
        function fgColor(column, row)
        {
            if (row === d.hoverIndex.row
                    && (column === d.hoverIndex.column || navigationMode === treeView.List)
                    && treeView.styleHints.foregroundHovered.a > 0)
                return treeView.styleHints.foregroundHovered
            else if (row === treeView.currentIndex.row)
                return treeView.styleHints.foregroundCurrent
            else if (row % 2)
                return treeView.styleHints.foregroundOdd
            else
                return treeView.styleHints.foregroundEven
        }
        
        function indicatorColor(column, row)
        {
            if (row === d.hoverIndex.row
                    && (column === d.hoverIndex.column || navigationMode === treeView.List)
                    && treeView.styleHints.indicatorHovered.a > 0)
                return treeView.styleHints.indicatorHovered
            else if (row === treeView.currentIndex.row)
                return treeView.styleHints.indicatorCurrent
            else
                return treeView.styleHints.indicator
        }
        
        function overlayColor()
        {
            if (d.hoverIndex.row === treeView.currentIndex.row
                    && (d.hoverIndex.column === treeView.currentIndex.column || navigationMode === treeView.List)
                    && treeView.styleHints.overlayHovered.a > 0)
                return treeView.styleHints.overlayHovered
            else
                return treeView.styleHints.overlay
            
        }
    }
}