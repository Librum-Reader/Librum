import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style


Item
{
    id: root
    property bool selected: false
    signal clicked
    
    implicitWidth: 22
    implicitHeight: 22
    
    
    Rectangle
    {
        anchors.fill: parent
        radius: width
        border.color: root.selected ? Style.colorBasePurple: Style.colorLightBorder2
        antialiasing: true
        
        
        Rectangle
        {
            visible: root.selected
            width: parent.width / 1.7
            height: parent.height / 1.7
            anchors.centerIn: parent
            color: Style.colorBasePurple
            radius: width
            antialiasing: true
        }
    }
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked: root.clicked()
    }
}