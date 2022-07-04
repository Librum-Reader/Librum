import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style


Item
{
    id: root
    required property ListView containingListview
    property bool selected: containingListview.currentItem === this
    required property string text
    required property int index
    property int padding : 10
    signal clicked(int index)
    signal hovered(int index)
    
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
            color: root.selected ? Style.colorSidebarMark : mouseArea.containsMouse ? Style.colorLightGray : Style.colorBackground
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
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        
        onClicked: root.clicked(root.index)
    }
}