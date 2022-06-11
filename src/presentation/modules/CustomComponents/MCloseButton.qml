import QtQuick
import QtQuick.Controls

Item
{
    id: root
    implicitWidth: 32
    implicitHeight: 32
    
    
    Pane
    {
        id: container
        anchors.fill: parent
        padding: 0
        background: Rectangle
        {
            color: properties.colorBackground
            border.color: properties.colorLightBorder
            radius: 4
            antialiasing: true
        }
        
        
        Image
        {
            anchors.centerIn: parent
            sourceSize.height: 12
            source: properties.iconCloseBlack
            fillMode: Image.PreserveAspectFit
        }
    }
}