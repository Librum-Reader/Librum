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
    readonly property alias pagetableView: tableView
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
        
        // Take over focus when clicked
        onClicked:
        {
            root.forceActiveFocus();
            mouse.accepted = false;
        }
        
        
        TableView
        {
            id: tableView
            readonly property int normalWidth: 1020
            readonly property int scrollSpeed: 1600
            
            height: parent.height /** 10*/  // Increase the height to render pages in advance
            width: contentWidth == 0 ? 1020 : contentWidth >= root.width 
                                       ? root.width : contentWidth
            anchors.centerIn: parent
            flickableDirection: Flickable.AutoFlickDirection
            contentWidth: 1020
            interactive: true
            rowSpacing: 14
            clip: true
            boundsMovement: Flickable.StopAtBounds
            flickDeceleration: 10000
            model: root.document.pageCount
            delegate: MPageView
            {
                implicitHeight: 1334 /*Math.round(width / pageRatio)*/
                implicitWidth: Math.round(height * pageRatio) /*tableView.contentWidth*/
                document: root.document
                pageNumber: modelData
            }
            
            
            // Set the book's current page once the model is loaded
//            onModelChanged: root.setPage(Globals.selectedBook.currentPage - 1)
//            onWidthChanged: root.pageWidthChanged(width)
            onContentYChanged: NavigationLogic.updateCurrentPageCounter();
            
            // A custom helper class which provides Qt6 like functions for
            // the TableView, such as 'itemAtCell(cell)'
            TableViewExtra
            {
                id: tableHelper
                tableView: tableView
            }
            
//            MouseArea
//            {
//                id: wheelInterceptor
//                anchors.fill: parent
                
//                onWheel:
//                {
//                    NavigationLogic.handleWheel(wheel);
//                    wheel.accepted = true;
//                }
                
//                onClicked:
//                {
//                    root.forceActiveFocus();
//                    mouse.accepted = false;
//                }
//            }
        }
    }
    
    Rectangle
    {
        z: 100
        color: "red"
        width: root.width
        height: 2
        y: tableView.height/2
    }
    
    
    function zoom(factor)
    {
        NavigationLogic.zoom(factor);
    }
    
    function flick(direction)
    {
        let up = direction === "up";
        NavigationLogic.flick(tableView.scrollSpeed * (up ? 1 : -1));
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