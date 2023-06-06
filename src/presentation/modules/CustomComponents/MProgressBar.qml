import QtQuick 2.15

Rectangle
{
    id: root
    property int progress
    
    implicitWidth: 100
    implicitHeight: 25
    radius: 6
    
    
    Rectangle
    {
        id: fillRect
        
        radius: root.radius
        width: root.width * (root.progress / 100)
        height: parent.height
        color: "orange"
    }
}