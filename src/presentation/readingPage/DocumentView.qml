import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import Librum.elements 1.0


QQC2.ScrollView
{
    id: root
    property DocumentItem document
    property PageItem page: mouseArea.currPageDelegate.pageItem
    signal clicked
    
    
    Flickable
    {
        id: flick
        interactive: false
        onWidthChanged: resizeTimer.restart()
        onHeightChanged: resizeTimer.restart()
        
        Component.onCompleted:
        {
            flick.contentWidth = flick.width
            flick.contentHeight = flick.width / mouseArea.currPageDelegate.pageRatio
        }
        Connections
        {
            target: root.document
            function onUrlChanged()
            {
                resizeTimer.restart()
            }
        }
        Timer
        {
            id: resizeTimer
            interval: 250
            onTriggered:
            {
                flick.contentWidth = flick.width
                flick.contentHeight = flick.width / mouseArea.currPageDelegate.pageRatio
            }
        }
        
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
            property Item currPageDelegate: page1
            property Item prevPageDelegate: page2
            property Item nextPageDelegate: page3
            
            onPressed:
            {
                var pos = mapToItem(flick, mouse.x, mouse.y);
                startMouseX = oldMouseX = pos.x;
                startMouseY = oldMouseY = pos.y;
            }
            onPositionChanged:
            {
                var pos = mapToItem(flick, mouse.x, mouse.y);
                
                flick.contentY = Math.max(0, Math.min(flick.contentHeight - flick.height, flick.contentY - (pos.y - oldMouseY)));
                
                if ((pos.x - oldMouseX > 0 && flick.atXBeginning) ||
                        (pos.x - oldMouseX < 0 && flick.atXEnd))
                {
                    currPageDelegate.x += pos.x - oldMouseX;
                    mouseArea.incrementing = currPageDelegate.x <= 0;
                } else
                {
                    flick.contentX = Math.max(0, Math.min(flick.contentWidth - flick.width, flick.contentX - (pos.x - oldMouseX)));
                }
                
                oldMouseX = pos.x;
                oldMouseY = pos.y;
            }
            
            onReleased: resetAnim.running = true;
            onCanceled: resetAnim.running = true;
            
            onClicked:
            {
                var pos = mapToItem(flick, mouse.x, mouse.y);
                if (Math.abs(startMouseX - pos.x) < 20 && Math.abs(startMouseY - pos.y) < 20)
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
                    resizeTimer.stop();
                } else
                {
                    flick.contentY = Math.min(flick.contentHeight-flick.height, Math.max(0, flick.contentY - wheel.angleDelta.y));
                }
            }
            
            PageView
            {
                id: page1
                document: root.document
                z: 2
            }
            PageView
            {
                id: page2
                document: root.document
                z: 1
            }
            PageView
            {
                id: page3
                document: root.document
                z: 0
            }
            
            
            Binding
            {
                target: mouseArea.currPageDelegate
                property: "pageNumber"
                value: root.document.currentPage
            }
            Binding
            {
                target: mouseArea.currPageDelegate
                property: "visible"
                value: true
            }
            
            Binding
            {
                target: mouseArea.prevPageDelegate
                property: "pageNumber"
                value: root.document.currentPage - 1
            }
            Binding
            {
                target: mouseArea.prevPageDelegate
                property: "visible"
                value: !mouseArea.incrementing && root.document.currentPage > 0
            }
            
            Binding
            {
                target: mouseArea.nextPageDelegate
                property: "pageNumber"
                value: root.document.currentPage + 1
            }
            Binding
            {
                target: mouseArea.nextPageDelegate
                property: "visible"
                value: mouseArea.incrementing && root.document.currentPage < document.pageCount-1
            }
            
            NumberAnimation
            {
                id: resetAnim
                target: mouseArea.currPageDelegate
                properties: "x"
                to: 0
                easing.type: Easing.InQuad
                duration: 250
            }
        }
    }
}