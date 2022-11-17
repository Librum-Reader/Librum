import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import Librum.elements 1.0
import Librum.style 1.0


Pane
{
    id: root
    property DocumentItem document
    readonly property alias pageListView: listView
    signal clicked
    
    padding: 0
    background: Rectangle
    {
        color: "transparent"
    }
    
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        
        onWheel: listView.handleWheel(wheel)
        
        
        ListView
        {
            id: listView
            readonly property int normalWidth: 1020
            
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
            
            
            function handleWheel(wheel)
            {
                // Generate factors between 0.8 and 1.2
                let factor = (((wheel.angleDelta.y / 120)+1) / 5 ) + 0.8;
                
                
                if (wheel.modifiers & Qt.ControlModifier)
                {
                    root.zoom(factor);
                }
                else if(wheel.button)
                {
                    if(factor > 1)
                        root.flick(2000);
                    else
                        root.flick(-2000);
                    
                    updateCurrentPageCounter();
                }
            }
            
            function updateCurrentPageCounter()
            {
                // Set current page
                let newPage = Math.round(listView.contentY / listView.currentItem.height);
                if(newPage != root.document.currentPage)
                    root.document.currentPage = newPage;
            }
            
            /**
             * Changes the current move direction of the listview, without actually
             * moving visibly. This is neccessary since the listview only chaches
             * delegates in the direction of the current move direction.
             * If we e.g. scroll downwards and then go to the previousPage
             * by setting the contentY, the previous pages are not cached
             * which might lead to visible loading while moving through the
             * book with the arrow keys
             */
            function setMoveDirection(direction)
            {
                if(direction === "up")
                {
                    listView.flick(0, -1000);
                    listView.cancelFlick();
                }
                else if(direction === "down")
                {
                    listView.flick(0, 1000);
                    listView.cancelFlick();
                }
            }
        }
    }
    
    function zoom(factor)
    {
        let newWidth = listView.contentWidth * factor;
        
        // Prevent from too high/low zooms
        if (newWidth < listView.normalWidth / 6 || newWidth > listView.normalWidth * 5)
        {
            return;
        }
        
        let mappedPoint = mapToItem(listView, Qt.point(mouseArea.mouseX, mouseArea.mouseY));
        listView.resizeContent(Math.round(newWidth), Math.round(newWidth / listView.currentItem.pageRatio),
                               mappedPoint);
        listView.returnToBounds();
    }
    
    function flick(factor)
    {
        listView.flick(0, factor);
        
        listView.updateCurrentPageCounter();
    }
    
    function nextPage()
    {
        let pageHeight = listView.currentItem.height;
        let currentPageStartY = root.document.currentPage * pageHeight;
        listView.contentY = currentPageStartY + pageHeight;
        
        listView.updateCurrentPageCounter();
        listView.setMoveDirection("up");
    }
    
    function previousPage()
    {
        let pageHeight = listView.currentItem.height;
        let currentPageStartY = root.document.currentPage * pageHeight;
        listView.contentY = currentPageStartY - pageHeight;
        
        listView.updateCurrentPageCounter();
        listView.setMoveDirection("down");
    }
}