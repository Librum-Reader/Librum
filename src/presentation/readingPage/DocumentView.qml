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
            cacheBuffer: 2000
            interactive: false
            boundsMovement: Flickable.StopAtBounds
            flickDeceleration: 10000
            currentIndex: 0
            model: root.document.pageCount
            
            onWidthChanged: toolbar.pageWidth = width
            
            // Some themes try to set interactive to true
            Component.onCompleted: listView.interactive = true
            
            
            delegate: PageView
            {
                width: listView.contentWidth
                height: Math.round(width / listView.currentItem.pageRatio)
                document: root.document
                pageNumber: modelData
                visible: true
            }
            
            MouseArea
            {
                anchors.fill: parent
                
                onWheel:
                {
                    listView.handleWheel(wheel);
                    wheel.accepted = true;
                }
            }
            
            
            
            function handleWheel(wheel)
            {
                // Generate factors between 0.8 and 1.2
                let factor = (((wheel.angleDelta.y / 120)+1) / 5 ) + 0.8;
                
                
                if (wheel.modifiers & Qt.ControlModifier)
                {
                    root.zoom(factor);
                }
                else
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
        }
    }
    
    
    function zoom(factor)
    {
        let newWidth = listView.contentWidth * factor;
        
        // Prevent a too high/low zooms
        if (newWidth < listView.normalWidth / 6 || newWidth > listView.normalWidth * 5)
        {
            return;
        }
        
        listView.resizeContent(Math.round(newWidth), Math.round(newWidth / listView.currentItem.pageRatio), Qt.point(0,0));
        listView.returnToBounds();
    }
    
    function flick(factor)
    {
        listView.flick(0, factor);
    }
    
    function nextPage()
    {
        let pageHeight = listView.currentItem.height;
        let currentPageStartY = root.document.currentPage * pageHeight;
        listView.contentY = currentPageStartY + pageHeight;
        
        listView.updateCurrentPageCounter();
    }
    
    function previousPage()
    {
        let pageHeight = listView.currentItem.height;
        let currentPageStartY = root.document.currentPage * pageHeight;
        listView.contentY = currentPageStartY - pageHeight;
        
        listView.updateCurrentPageCounter();
    }
}