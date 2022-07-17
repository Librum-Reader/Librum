import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.icons


Item
{
    id: root
    property bool selected: false
    
    implicitWidth: 24
    implicitHeight: 24
    
    
    Rectangle
    {
        anchors.fill: parent
        radius: width
        border.color: root.selected ? Style.colorBasePurple: Style.colorLightBorder2
        
        
        Rectangle
        {
            visible: root.selected
            width: parent.width / 1.7
            height: parent.height / 1.7
            anchors.centerIn: parent
            color: Style.colorBasePurple
            radius: width
        }
    }
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked: root.selected = !root.selected;
    }
}