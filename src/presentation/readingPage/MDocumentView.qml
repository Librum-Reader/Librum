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
    readonly property alias pageListView: listView
    signal clicked
    signal pageWidthChanged(int width)
    
    padding: 0
    background: Rectangle { color: "transparent" }
    
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        
        // Handle scrolling customly
        onWheel: NavigationLogic.handleWheel(wheel)
        
        onClicked:
        {
            root.forceActiveFocus();
            mouse.accepted = false;
        }
        
        
        ListView
        {
            id: listView
            readonly property int normalWidth: 1020
            readonly property int scrollSpeed: 1600
            
            height: parent.height
            width: contentWidth == 0 ? 1020 : contentWidth >= root.width 
                                       ? root.width : contentWidth
            anchors.centerIn: parent
            flickableDirection: Flickable.AutoFlickDirection
            contentWidth: 1020
            cacheBuffer: 5000  // Load some pages in advance
            interactive: true
            boundsMovement: Flickable.StopAtBounds
            flickDeceleration: 10000
            currentIndex: 0
            model: root.document.pageCount
            delegate: MPageView
            {
                width: listView.contentWidth
                height: Math.round(width / pageRatio) + pageSpacing
                document: root.document
                pageNumber: modelData
                container: listView
            }
            
            // Set the book's current page once the model is loaded
            onModelChanged:
            {
                if(listView.currentItem != null)
                    root.setPage(Globals.selectedBook.currentPage - 1)
            }
            onWidthChanged: root.pageWidthChanged(width)
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
                
                onClicked:
                {
                    root.forceActiveFocus();
                    mouse.accepted = false;
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
        NavigationLogic.flick(listView.scrollSpeed * (up ? 1 : -1));
    }
    
    function nextPage()
    {
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