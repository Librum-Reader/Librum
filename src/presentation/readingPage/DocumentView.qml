import QtQuick 2.15
import QtQuick.Controls 2.15
import Librum.elements 1.0


Item
{
    id: root
    property DocumentItem document
    readonly property real contentWidth: flick.contentWidth
    readonly property real contentHeight: flick.contentHeight
    signal clicked
    
    
    // Some themes try to set interactive to ture, this is to make sure its
    // always false, since we need to handle dragging by ourselves
    Component.onCompleted: flick.interactive = false
    Flickable
    {
        id: flick
        property bool firstHappend: false
        property int startWidth: 0
        
        interactive: false
        anchors.fill: parent
        contentWidth: startWidth
        contentHeight: startWidth / listView.currentItem.pageRatio
        
        onWidthChanged:
        {
            if(firstHappend)
                return;
            
            startWidth = root.parent.width;
            firstHappend = true;
        }
        
        
        MouseArea
        {
            id: mouseArea
            anchors.top: parent.top
            width: flick.width > flick.contentWidth ? flick.width : flick.contentWidth
            height: flick.height > flick.contentHeight ? flick.height : flick.contentHeight
            
            
            property real oldMouseX
            property real oldMouseY
            property real startMouseX
            property real startMouseY
            property bool incrementing: true
            property Item currPageDelegate: page
            
            
            onPressed:
            {
                var pos = mapToItem(flick, mouse.x, mouse.y);
                startMouseX = oldMouseX = pos.x;
                startMouseY = oldMouseY = pos.y;
            }
            
            onPositionChanged:
            {
                var pos = mapToItem(flick, mouse.x, mouse.y);
                
                // Move on Y-Axis
                let bottomClamp = flick.contentHeight - flick.height;
                let topClamp = 0;
                
                let yFlickAmount = flick.contentY - (pos.y - oldMouseY);
                let amountToMoveOnY = Math.min(bottomClamp, yFlickAmount);
                flick.contentY = Math.max(topClamp, amountToMoveOnY);
                
                
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
                var factor = (((wheel.angleDelta.y / 120)+1) / 5 ) + 0.8;
                
                if (wheel.modifiers & Qt.ControlModifier)
                {
                    var newWidth = flick.contentWidth * factor;
                    var newHeight = flick.contentHeight * factor;
                    
                    if (newHeight < flick.height - 500 || newHeight > flick.height * 5)
                    {
                        return;
                    }
                    
                    flick.resizeContent(newWidth, newHeight, Qt.point(wheel.x, wheel.y));
                    flick.returnToBounds();
                }
                else
                {
                    if(factor > 1)
                        listView.flick(0, 2000)
                    else
                        listView.flick(0, -2000)
                }
            }
            
            
            Component.onCompleted: listView.interactive = false
            ListView
            {
                id: listView
                cacheBuffer: 1000
                anchors.fill: parent
                interactive: false
                boundsMovement: Flickable.StopAtBounds
                clip: false
                flickDeceleration: 7000
                
                model: root.document.pageCount
                
                delegate: PageView
                {
                    id: page
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