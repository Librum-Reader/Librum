import QtQuick 2.15
import QtQuick.Controls 2.15
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
        property real oldMouseX
        property real oldMouseY
        property real startMouseX
        property real startMouseY
        property bool incrementing: true
        
        anchors.fill: parent
        
        onPressed:
        {
            var pos = mapToItem(flick, mouse.x, mouse.y);
            startMouseX = oldMouseX = pos.x;
            startMouseY = oldMouseY = pos.y;
        }
        
        onPositionChanged:
        {
            var pos = mapToItem(flick, mouse.x, mouse.y);
            
            // Move on X-Axis
            let leftClamp = 0;
            let rightClamp = flick.contentWidth - flick.width;
            
            let xFlickAmount = flick.contentX - (pos.x - oldMouseX);
            let amountToMoveOnX = Math.min(rightClamp, xFlickAmount);
            flick.contentX = Math.max(leftClamp, amountToMoveOnX);
            
            
            oldMouseX = pos.x;
            oldMouseY = pos.y;
        }
        
        onWheel:
        {
            //generate factors between 0.8 and 1.2
            let factor = (((wheel.angleDelta.y / 120)+1) / 5 ) + 0.8;
            
            if (wheel.modifiers & Qt.ControlModifier)
            {
                let newWidth = flick.contentWidth * factor;
                let newHeight = flick.contentHeight * factor;
                
                // restraining from a too high zoom
                if (newHeight < flick.height - 500 || newHeight > flick.height * 5)
                {
                    return;
                }
                
                flick.resizeContent(newWidth, newHeight, Qt.point(0, 0));
                flick.returnToBounds();
            }
            else
            {
                if(factor > 1)
                    listView.flick(0, 2000)
                else
                    listView.flick(0, -2000)
                
                
                // set current page
                let newPage = Math.round(listView.contentY / flick.contentHeight);
                if(newPage != root.document.currentPage)
                    root.document.currentPage = newPage;
            }
        }
        
        
        Rectangle
        {
            id: pageArea
            height: parent.height
            width: flick.contentWidth == 0 ? 1020 : flick.contentWidth >= parent.width
                                             ? parent.width : flick.contentWidth
            anchors.centerIn: parent
            color: Style.colorBackground
            radius: 2
            
            onWidthChanged: toolbar.pageWidth = width
            
            // Some themes try to set interactive to true
            Component.onCompleted: flick.interactive = false
            
            Flickable
            {
                id: flick
                property int defaultContentWidth: 1020
                
                height: parent.height
                width: contentWidth == 0 ? 1020 : contentWidth >= root.width 
                                           ? root.width : contentWidth
                interactive: false
                contentWidth: defaultContentWidth
                contentHeight: Math.round(defaultContentWidth / listView.currentItem.pageRatio)
                boundsMovement: Flickable.StopAtBounds
                flickDeceleration: 10000
                clip: true
                
                
                ListView
                {
                    id: listView
                    cacheBuffer: 4000
                    width: parent.width
                    height: flick.height
                    interactive: false
                    boundsMovement: Flickable.StopAtBounds
                    clip: false
                    flickDeceleration: 10000
                    currentIndex: 0
                    
                    
                    model: root.document.pageCount
                    
                    delegate: PageView
                    {
                        width: flick.contentWidth
                        height: flick.contentHeight
                        document: root.document
                        pageNumber: modelData
                        visible: true
                    }
                }
            }
        }
    }
}