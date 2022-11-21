import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import Librum.elements 1.0
import Librum.style 1.0
import "DocumentNavigation.js" as NavigationLogic


Pane
{
    id: root
    property DocumentItem document
    readonly property alias pageListView: listView
    signal clicked
    
    padding: 0
    background: Rectangle { color: "transparent" }
    
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        
        onWheel: NavigationLogic.handleWheel(wheel)
        
        
        ListView
        {
            id: listView
            readonly property int normalWidth: 1020
            readonly property int scrollSpeed: 1600
            
            onContentYChanged: NavigationLogic.updateCurrentPageCounter();
            
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
            
            onWidthChanged: toolbar.pageWidth = width
            
            delegate: PageView
            {
                width: listView.contentWidth
                height: Math.round(width / pageRatio) + pageSpacing
                document: root.document
                pageNumber: modelData
                container: listView
            }
            
            
            MouseArea
            {
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
        NavigationLogic.flick(listView.scrollSpeed * (up ? 1 : -1));
    }
    
    function nextPage()
    {
        NavigationLogic.nextPage();
    }
    
    function previousPage()
    {
        NavigationLogic.previousPage();
    }
}