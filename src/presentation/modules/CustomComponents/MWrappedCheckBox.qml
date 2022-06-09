import QtQuick
import QtQuick.Controls
import CustomComponents


Item
{
    id: root
    property alias activated : innerCheckBox.checked
    signal checked
    
    implicitWidth: 40
    implicitHeight: 36
    
    Pane
    {
        id: container
        anchors.fill: parent
        padding: 0
        background: Rectangle
        {
            color: properties.colorBackground
            border.width: 1
            border.color: properties.colorLightBorder
            radius: 5
        }
        
        
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
    
    
    function giveFocus()
    {
        innerCheckBox.giveFocus();
    }    
}
