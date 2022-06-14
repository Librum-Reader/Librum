import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style


Item
{
    id: root
    property bool selected : false
    property string text : "My item"
    property int padding : 10
    signal clicked
    
    implicitWidth: 137
    implicitHeight: 36
    
    
    Pane
    {
        id: container
        anchors.fill: parent
        verticalPadding: 0
        horizontalPadding: root.padding
        background: Rectangle
        {
            color: (root.selected) ? Style.colorSidebarMark : Style.colorBackground
            radius: 4
            antialiasing: true
        }
        
        
        Label
        {
            id: content
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            leftPadding: 4
            color: (root.selected) ? Style.colorBasePurple : Style.colorLightText3
            text: root.text
            font.pointSize: 10.75
            
            font.family: Style.defaultFontFamily
            font.weight: Font.DemiBold
        }
    }
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked: root.clicked()
    }
}