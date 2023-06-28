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
    property DocumentItem document
    readonly property alias pagepageView: pageView
    signal clicked
    signal zoomFactorChanged(real factor)
    
    padding: 0
    background: Rectangle { color: "transparent" }
    
    
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
            readonly property real defaultPageHeight: 1310
            property real zoomFactor: 1
            readonly property int scrollSpeed: 5500
            property int pageSpacing: pageView.getPageSpacing(zoomFactor)
            
            height: parent.height
            width: currentItem.width <= root.width ? currentItem.width : root.width
            contentWidth: currentItem.width
            anchors.centerIn: parent
            flickableDirection: Flickable.AutoFlickDirection
            flickDeceleration: 100000
            interactive: true
            clip: true
            cacheBuffer: 20000
            maximumFlickVelocity: scrollSpeed
            boundsMovement: Flickable.StopAtBounds
            boundsBehavior: Flickable.StopAtBounds
            model: root.document.pageCount
            spacing: pageSpacing
            delegate: MPageView
            {
                height: Math.round(pageView.defaultPageHeight * pageView.zoomFactor)
                width: adaptedWidth
                
                document: root.document
                pageNumber: modelData
            }
            
            
            // Set the book's current page once the model is loaded
            onModelChanged: root.setPage(Globals.selectedBook.currentPage - 1)
            onContentYChanged: NavigationLogic.updateCurrentPageCounter();
            onZoomFactorChanged: root.zoomFactorChanged(pageView.zoomFactor)
            
            
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
    
    function setPage(pageNumber)
    {
        NavigationLogic.setPage(pageNumber);
    }
}