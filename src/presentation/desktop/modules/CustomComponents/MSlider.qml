import QtQuick 
import Librum.style

Rectangle
{
    id: root
    property alias value: handle.x
    signal controlledValueChanged(int value)
    
    onValueChanged: {
        if(valueChangeTimer.running)
            valueChangeTimer.stop()
        
        valueChangeTimer.start()
    }
    
    implicitWidth: 255
    implicitHeight: 9
    radius: 4
    color: "#5C4675"
    
    Rectangle
    {
        id: fill
        height: parent.height
        anchors.right: handle.right
        anchors.left: parent.left
        radius: parent.radius
        color: Style.colorBasePurple
    }
    
    Rectangle
    {
        id: handle
        z: 2
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height * 2.4
        width: parent.height * 0.7
        radius: 3
        color: Style.colorBasePurple
        antialiasing: true
        
        MouseArea
        {
            id: dragger
            anchors.fill: parent
            drag.axis: Drag.XAxis
            drag.target: handle
            drag.minimumX: 0
            drag.maximumX: root.width
            cursorShape: Qt.DragMoveCursor
            
            onPressed: root.forceActiveFocus()
        }
    }
    
    // To not emit "value changed" every time the slider is dragged for each pixel, we use a timer
    // that waits a certain amount of time before emitting the signal.
    Timer
    {
        id: valueChangeTimer
        interval: 300
        repeat: false
        
        onTriggered: root.controlledValueChanged(root.value)
    }
}