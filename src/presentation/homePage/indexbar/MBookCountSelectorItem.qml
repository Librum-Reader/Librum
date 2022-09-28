import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0


Item
{
    id: root
    required property ListView containingListview
    property bool selected: containingListview.currentItem === this
    required property int text
    required property int index
    property int padding : 10
    signal clicked(int index)
    
    implicitHeight: 36
    implicitWidth: 60
    
    
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
            anchors.centerIn: parent
            color: (root.selected) ? Style.colorBasePurple : Style.colorLightText3
            text: root.text
            font.pointSize: 10.75
            
            font.weight: root.selected ? Font.Bold : Font.DemiBold
        }
    }
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        
        onClicked: root.clicked(index)
    }
}