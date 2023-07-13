import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import Librum.elements 1.0
import Librum.style 1.0
import Librum.globals 1.0
import Librum.controllers 1.0
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
        root.setPage(Globals.selectedBook.currentPage - 1);
    }
    
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        
        // Handle scrolling customly
        onWheel: NavigationLogic.handleWheel(wheel)
        
        // Take over focus when clicked
        onClicked:
        {
            root.forceActiveFocus();
            mouse.accepted = false;
        }
        
        
        ListView
        {
            id: pageView
            readonly property int scrollSpeed: 5500
            property int pageSpacing: pageView.getPageSpacing(root.document.zoom)
            
            height: parent.height
            width: currentItem.implicitWidth <= root.width ? currentItem.implicitWidth : root.width
            contentWidth: currentItem.implicitWidth
            anchors.centerIn: parent
            flickableDirection: Flickable.AutoFlickDirection
            flickDeceleration: 100000
            interactive: true
            clip: true
            cacheBuffer: 1000
            maximumFlickVelocity: scrollSpeed
            boundsMovement: Flickable.StopAtBounds
            boundsBehavior: Flickable.StopAtBounds
            model: root.document.pageCount
            spacing: pageSpacing
            delegate: PageItem
            {
                pageNumber: modelData
                document: documentItem
                height: implicitHeight
                width: implicitWidth
            }
            
            
            // Set the book's current page once the model is loaded
            onContentYChanged: NavigationLogic.updateCurrentPageCounter();
            
            
            MouseArea
            {
                id: wheelInterceptor
                anchors.fill: parent
                
                onWheel:
                {
                    NavigationLogic.handleWheel(wheel);
                    wheel.accepted = true;
                }
            }
            
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
        let newZoomFactor = pageView.zoomFactor * factor;
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
        pageView.contentY += yOffset * root.document.zoom;
        console.log(yOffset);
    }
}