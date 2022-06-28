import QtQuick
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons


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
            checkedBorderWidth: 1
            checkedBackgroundColor: "transparent"
            imagePath: Icons.checkBlackThin
            borderWidth: 1
            
            onClicked: root.checked();
        }
    }
    
    
    function giveFocus()
    {
        innerCheckBox.giveFocus();
    }    
}
