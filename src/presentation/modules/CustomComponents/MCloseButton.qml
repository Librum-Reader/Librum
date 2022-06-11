import QtQuick
import QtQuick.Controls

Item
{
    id: root
    implicitWidth: 32
    implicitHeight: 32
    signal clicked
    
    
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
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked: root.clicked()
    }
}