import QtQuick
import QtQuick.Controls

Item
{
    id: root
    property double backgroundOpacityOnPressed : 0.7
    signal clicked
    
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
            opacity: (mouseArea.pressed || root.activeFocus ? root.backgroundOpacityOnPressed : 1)
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
        id: mouseArea
        anchors.fill: parent
        
        onClicked: root.clicked()
    }
}