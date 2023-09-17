import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.elements
import Librum.style
import Librum.globals
import Librum.controllers
import "DocumentNavigation.js" as NavigationLogic


/*
  A view on the document's pages in a certain layout (e.g. vertical)
  */
Pane {
    id: root
    signal clicked
    signal zoomFactorChanged(real factor)
    
    padding: 0
    background: Rectangle {
        color: "transparent"
    }
    // Disable pressing tab to focus other elements
    Keys.onTabPressed: event => {
                           event.accepted = true
                       }
    
    Component.onCompleted: {
        BookController.zoom = SettingsController.appearanceSettings.DefaultZoom / 100
    }
    
    Component.onDestruction: {
        BookController.zoom = 1
    }
    
    Connections {
        target: BookController
        
        function onGoToPosition(pageNumber, y) {
            root.setPage(pageNumber, y)
        }
        
        function onTextSelectionFinished(centerX, topY)
        {
            selectionOptionsPopup.highlight = "";
            
            selectionOptionsPopup.highlightCenterX = centerX;
            selectionOptionsPopup.highlightTopY = topY;
            internal.openPopupAt(selectionOptionsPopup, centerX, topY);
        }
        
        function onHighlightSelected(centerX, topY, highlightUuid)
        {
            selectionOptionsPopup.highlight = highlightUuid;
            
            selectionOptionsPopup.highlightCenterX = centerX;
            selectionOptionsPopup.highlightTopY = topY;
            internal.openPopupAt(selectionOptionsPopup, centerX, topY);
        }
        
        function onZoomChanged(newZoom)
        {
            let normMaxWidth = pageView.widestItem / pageView.prevZoom;
            pageView.widestItem = normMaxWidth * BookController.zoom;
            
            pageView.prevZoom = BookController.zoom;
        }
    }
    
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        
        // Handle scrolling customly
        onWheel: NavigationLogic.handleWheel(wheel)
        
        onPressed: mouse.accepted = false
        onReleased: mouse.accepted = false
        
        ListView {
            id: pageView
            readonly property int scrollSpeed: 5500
            property int pageSpacing: pageView.getPageSpacing(BookController.zoom)
            property int widestItem: 0
            property real prevZoom: 1
            
            height: parent.height
            width: contentWidth <= root.width ? contentWidth : root.width
            contentWidth: pageView.widestItem
            anchors.centerIn: parent
            flickableDirection: Flickable.AutoFlickDirection
            flickDeceleration: 100000
            interactive: false
            clip: true
            cacheBuffer: 1000
            maximumFlickVelocity: scrollSpeed
            boundsMovement: Flickable.StopAtBounds
            boundsBehavior: Flickable.StopAtBounds
            model: BookController.pageCount
            spacing: pageSpacing
            delegate: PageView {
                id: page
                pageNumber: modelData
                bookController: BookController
                height: implicitHeight
                width: implicitWidth
                colorInverted: SettingsController.appearanceSettings.PageColorMode === "Inverted"
                anchors.horizontalCenter: if (parent != null)
                                              parent.horizontalCenter
                
                Component.onCompleted: {
                    if(implicitWidth > pageView.contentWidth)
                        pageView.widestItem = page.implicitWidth;
                }
            }
            
            // Set the book's current page once the model is loaded
            onContentYChanged: {
                NavigationLogic.updateCurrentPageCounter()
                selectionOptionsPopup.close();
                colorSelectionPopup.close();
            }
            
            onContentXChanged: {
                selectionOptionsPopup.close();
                colorSelectionPopup.close();
            }
            
            Component.onCompleted: root.setPage(
                                       Globals.selectedBook.currentPage - 1)
            
            function getPageSpacing(zoom) {
                return Math.round(
                            SettingsController.appearanceSettings.PageSpacing
                            * (zoom + 0.4 * (1 - zoom)))
            }
        }
    }
    
    ScrollBar {
        id: verticalScrollbar
        width: hovered ? 14 : 12
        hoverEnabled: true
        active: true
        policy: ScrollBar.AlwaysOn
        orientation: Qt.Vertical
        size: pageView.height / pageView.contentHeight
        minimumSize: 0.04
        position: (pageView.contentY - pageView.originY) / pageView.contentHeight
        onPositionChanged: pageView.contentY = position * pageView.contentHeight + pageView.originY
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        horizontalPadding: 4
        
        contentItem: Rectangle {
            color: Style.colorScrollBarHandle
            opacity: verticalScrollbar.pressed ? 0.8 : 1
            radius: 4
        }
        
        background: Rectangle {
            implicitWidth: 26
            implicitHeight: 200
            color: verticalScrollbar.hovered ? Style.colorContainerBackground : "transparent"
        }
    }
    
    ScrollBar {
        id: horizontalScrollbar
        height: hovered ? 12 : 10
        hoverEnabled: true
        active: true
        policy: ScrollBar.AlwaysOn
        visible: pageView.contentWidth > pageView.width
        orientation: Qt.Horizontal
        size: pageView.width / pageView.contentWidth
        minimumSize: 0.04
        position: (pageView.contentX - pageView.originX) / pageView.contentWidth
        onPositionChanged: pageView.contentX = position * pageView.contentWidth + pageView.originX
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        horizontalPadding: 4
        
        contentItem: Rectangle {
            color: Style.colorScrollBarHandle
            opacity: horizontalScrollbar.pressed ? 0.8 : 1
            radius: 4
        }
        
        background: Rectangle {
            implicitWidth: 26
            implicitHeight: 200
            color: "transparent"
        }
    }
    
    MSelectionOptionsPopup {
        id: selectionOptionsPopup
        property real highlightCenterX
        property real highlightTopY
        
        onHighlightOptionSelected: 
            (uuid) => {
                if(selectionOptionsPopup.highlight == "")
                    colorSelectionPopup.highlight = uuid;
                else
                    colorSelectionPopup.highlight = selectionOptionsPopup.highlight
                
                internal.openPopupAt(colorSelectionPopup, 
                                     selectionOptionsPopup.highlightCenterX, 
                                     selectionOptionsPopup.highlightTopY);
            }
    }
    
    MColorSelectionPopup {
        id: colorSelectionPopup
    }
    
    function zoom(factor) {
        NavigationLogic.zoom(factor)
    }
    
    function changeZoomBy(factor) {
        let newZoomFactor = BookController.zoom * factor
        NavigationLogic.zoom(newZoomFactor)
    }
    
    function flick(direction) {
        let up = direction === "up"
        NavigationLogic.flick(0, (pageView.scrollSpeed / 1.4) * (up ? 1 : -1))
    }
    
    function nextPage() {
        // Prevent trying to go over the end
        let newPage = BookController.currentPage + 1
        if (newPage > BookController.pageCount - 1)
            return
        
        NavigationLogic.setPage(BookController.currentPage + 1)
    }
    
    function previousPage() {
        NavigationLogic.setPage(BookController.currentPage - 1)
    }
    
    function setPage(pageNumber, yOffset = 0) {
        NavigationLogic.setPage(pageNumber)
        
        let space = 10
        pageView.contentY += yOffset * root.BookController.zoom - space
    }
    
    QtObject
    {
        id: internal
        
        function openPopupAt(popup, centerX, topY)
        {
            let pageYOffset = pageView.contentY - activeFocusItem.y;
            let pageXOffset = pageView.contentX - activeFocusItem.x;
            let posX = centerX + pageView.x - popup.width / 2 - pageXOffset;
            let posY = topY - popup.height - pageYOffset - 6;
            
            let spaceToRight = (pageView.x + pageView.width) - (posX + popup.width)
            if(spaceToRight < 0)
                posX = posX + spaceToRight;
            
            let spaceToLeft = (pageView.x + pageView.width) - (posX + popup.width)
            
            popup.x = posX;
            popup.y = posY;
            
            popup.open()
        }
    }
}
