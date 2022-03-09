import QtQuick
import "../"


FocusScope
{
    id: root
    property alias activated : innerCheckBox.activated
    signal checked

    implicitWidth: 40
    implicitHeight: 36
    
    Rectangle
    {
        id: container
        anchors.fill: parent
        
        color: properties.colorBackground
        border.width: 1
        border.color: properties.colorLightBorder
        radius: 5
        
        MCheckBox
        {
            id: innerCheckBox
            width: 22
            height: 22
            anchors.centerIn: parent
            borderColor: properties.colorLightBorder2
            borderWidth: 1
            
            onClicked: root.checked();
        }
    }
}
