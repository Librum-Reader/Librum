import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import Librum.elements 1.0
import Librum.style 1.0
import Librum.globals 1.0
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
        
        
        GridView
        {
            id: pageView
            readonly property real defaultHeight: 1334
            property real zoomFactor: 1
            readonly property int defaultPageSpacing: 12
            readonly property int scrollSpeed: 1600
            
            height: parent.height
            width: contentItem.childrenRect.width > root.width ? root.width : contentItem.childrenRect.width
            contentWidth: currentItem.width
            anchors.centerIn: parent
            flickableDirection: Flickable.VerticalFlick
            interactive: true
            clip: true
            cellHeight: Math.round(pageView.defaultHeight * pageView.zoomFactor)
            cellWidth: currentItem.width
            cacheBuffer: 20000
            flow: GridView.FlowLeftToRight
            boundsMovement: Flickable.StopAtBounds
            flickDeceleration: 10000
            model: root.document.pageCount
            delegate: MPageView
            {
                // The width is automatically deduced
                height: Math.round(pageView.defaultHeight * pageView.zoomFactor)
                width: adaptedWidth
                
                document: root.document
                pageNumber: modelData
                pageSpacing: pageView.defaultPageSpacing
            }
            
            
            // Set the book's current page once the model is loaded
            onModelChanged: root.setPage(Globals.selectedBook.currentPage - 1)
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
        }
    }
    
    
    function zoom(factor)
    {
        NavigationLogic.zoom(factor);
    }
    
    function flick(direction)
    {
        let up = direction === "up";
        NavigationLogic.flick(pageView.scrollSpeed * (up ? 1 : -1));
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