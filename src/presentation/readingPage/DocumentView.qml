import QtQuick 2.15
import QtQuick.Controls 2.15
import Librum.elements 1.0


ScrollView
{
    id: root
    property DocumentItem document
    property PageItem page: page.pageItem
    signal clicked
    
    
    // Some themes try to set interactive to ture, this is to make sure its
    // always false, since we need to handle dragging by ourselves
    Component.onCompleted: flick.interactive = false
    Flickable
    {
        id: flick
        interactive: false
        contentWidth: flick.width
        contentHeight: flick.width / page.pageRatio
        clip: true
        
        
        MouseArea
        {
            id: mouseArea
            width: parent.width
            height: parent.height
            
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
            
            onDoubleClicked:
            {
                flick.contentWidth = flick.width
                flick.contentHeight = flick.width / mouseArea.currPageDelegate.pageRatio
            }
            
            onClicked:
            {
                var pos = mapToItem(flick, mouse.x, mouse.y);
                if (Math.abs(startMouseX - pos.x) < 20 &&
                        Math.abs(startMouseY - pos.y) < 20)
                {
                    root.clicked();
                }
            }
            
            onWheel:
            {
                if (wheel.modifiers & Qt.ControlModifier)
                {
                    //generate factors between 0.8 and 1.2
                    var factor = (((wheel.angleDelta.y / 120)+1) / 5 )+ 0.8;
                    
                    var newWidth = flick.contentWidth * factor;
                    var newHeight = flick.contentHeight * factor;
                    
                    if (newWidth < flick.width || newHeight < flick.height ||
                            newHeight > flick.height * 3)
                    {
                        
                        return;
                    }
                    
                    flick.resizeContent(newWidth, newHeight, Qt.point(wheel.x, wheel.y));
                    flick.returnToBounds();
                }
                else
                {
                    flick.contentY = Math.min(flick.contentHeight-flick.height, Math.max(0, flick.contentY - wheel.angleDelta.y));
                }
            }
            
            PageView
            {
                id: page
                document: root.document
                visible: true
                z: 2
            }
            
            
            Binding
            {
                target: page
                property: "pageNumber"
                value: root.document.currentPage
            }
        }
    }
}