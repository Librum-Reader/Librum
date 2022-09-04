import QtQuick 2.15
import QtQuick.Controls 2.15
import Librum.style 1.0
import CustomComponents 1.0


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
            color: Style.colorBackground
            border.width: 1
            border.color: Style.colorLightBorder
            radius: 5
        }
        
        
        MCheckBox
        {
            id: innerCheckBox
            width: 22
            height: 22
            anchors.centerIn: parent
            borderColor: Style.colorLightBorder2
            borderWidth: 1
            imageSize: 12
            
            onClicked: root.checked();
        }
    }
    
    
    function giveFocus()
    {
        innerCheckBox.giveFocus();
    }    
}
