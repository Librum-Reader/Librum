import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Librum.elements
import Librum.style
import Librum.globals
import Librum.controllers
import "DocumentNavigation.js" as NavigationLogic

/*
  A view on the document's pages in a certain layout (e.g. vertical)
  */
Pane
{
    id: root
    property var document
    signal clicked
    signal zoomFactorChanged(real factor)
    
    padding: 0
    background: Rectangle { color: "transparent" }
    
    Component.onCompleted: 
    {
        root.document.zoom = SettingsController.appearanceSettings.DefaultZoom / 100;
    }
    
    Keys.onTabPressed: (event) => { event.accepted = true; } // Disable pressing tab to focus other elements
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        
        // Handle scrolling customly
        onWheel: NavigationLogic.handleWheel(wheel)
        
        onPressed: mouse.accepted = false
        onReleased: mouse.accepted = false
        
        
        ListView
        {
            id: pageView
            readonly property int scrollSpeed: 5500
            property int pageSpacing: pageView.getPageSpacing(root.document.zoom)
            
            height: parent.height
            width: if(currentItem) currentItem.implicitWidth <= root.width ? currentItem.implicitWidth : root.width
            contentWidth: currentItem.implicitWidth
            anchors.centerIn: parent
            flickableDirection: Flickable.AutoFlickDirection
            flickDeceleration: 100000
            interactive: false
            clip: true
            cacheBuffer: 1000
            maximumFlickVelocity: scrollSpeed
            boundsMovement: Flickable.StopAtBounds
            boundsBehavior: Flickable.StopAtBounds
            model: root.document.pageCount
            spacing: pageSpacing
            delegate: PageItem
            {
                id: page
                pageNumber: modelData
                document: documentItem
                height: implicitHeight
                width: implicitWidth
                colorInverted: SettingsController.appearanceSettings.PageColorMode === "Inverted"
                anchors.horizontalCenter: if(parent != null) parent.horizontalCenter
                
                Keys.onPressed: (event) =>
                                {
                                    if(event.key === Qt.Key_C && (event.modifiers & Qt.ControlModifier))
                                    {
                                        page.copySelectedText();
                                    }
                                }
                
                MouseArea
                {
                    anchors.fill: parent
                    hoverEnabled: true
                    property var selectionStartPos
                    property var selectionEndPos
                    
                    onWheel:
                    {
                        NavigationLogic.handleWheel(wheel);
                        wheel.accepted = true;
                    }
                    
                    onClicked: mouse.accepted = false;
                    
                    onPressed:
                    {
                        root.forceActiveFocus();
                        page.removeSelection();
                        selectionStartPos = Qt.point(mouseX, mouseY)
                        
                        mouse.accepted = true;
                    }
                    onPositionChanged:
                    {
                        let textBelowCursor = page.pointIsAboveText(mouseX, mouseY);
                        if(textBelowCursor)
                            cursorShape = Qt.IBeamCursor;
                        else
                            cursorShape = Qt.ArrowCursor;
                        
                        if(!pressed)
                            return;
                        
                        selectionEndPos = Qt.point(mouseX, mouseY);
                        page.forceActiveFocus();
                        page.setSelection(selectionStartPos.x, selectionStartPos.y, 
                                          selectionEndPos.x, selectionEndPos.y)
                    }
                }
            }
            
            
            // Set the book's current page once the model is loaded
            onContentYChanged: NavigationLogic.updateCurrentPageCounter();
            Component.onCompleted: root.setPage(Globals.selectedBook.currentPage - 1);
            
            function getPageSpacing(zoom)
            {
                return Math.round(SettingsController.appearanceSettings.PageSpacing * (zoom + 0.4 * (1 - zoom)));
            }
        }
    }
    
    ScrollBar
    {
        id: scrollbar
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
        
        contentItem: Rectangle
        {
            color: Style.colorScrollBarHandle
            opacity: scrollbar.pressed ? 0.8 : 1
            radius: 4
        }
        
        background: Rectangle
        {
            implicitWidth: 26
            implicitHeight: 200
            color: scrollbar.hovered ? Style.colorContainerBackground : "transparent"
        }
    }
    
    
    function zoom(factor)
    {
        NavigationLogic.zoom(factor);
    }
    
    function changeZoomBy(factor)
    {
        let newZoomFactor = root.document.zoom * factor;
        NavigationLogic.zoom(newZoomFactor);
    }
    
    function flick(direction)
    {
        let up = direction === "up";
        NavigationLogic.flick((pageView.scrollSpeed / 1.4) * (up ? 1 : -1));
    }
    
    function nextPage()
    {
        // Prevent trying to go over the end
        let newPage = root.document.currentPage + 1;
        if(newPage > root.document.pageCount - 1)
            return;
        
        NavigationLogic.setPage(root.document.currentPage + 1);
    }
    
    function previousPage()
    {
        NavigationLogic.setPage(root.document.currentPage - 1);
    }
    
    function setPage(pageNumber, yOffset = 0)
    {
        NavigationLogic.setPage(pageNumber);
        
        let space = 10;
        pageView.contentY += yOffset * root.document.zoom - space;
    }
}
