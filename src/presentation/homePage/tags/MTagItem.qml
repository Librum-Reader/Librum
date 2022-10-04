import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.globals 1.0


Item
{
    id: root
    required property int index
    property bool selected: false
    signal removeTag(int index)
    
    implicitHeight: 38
    implicitWidth: 400
    
    
    Pane
    {
        id: container
        anchors.fill: parent
        padding: 0
        background: Rectangle
        {
            color: root.selected ? Style.colorSidebarMark : 
                                   mouseArea.containsMouse || ListView.currentIndex === index
                                   ? Style.colorLightGray : Style.colorBackground
            radius: 4
        }
        
        
        RowLayout
        {
            id: layout
            anchors.fill: parent
            spacing: 4
            
            
            Label
            {
                id: text
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.leftMargin: 12
                verticalAlignment: Text.AlignVCenter
                text: Globals.bookTags[root.index].name
                font.weight: root.selected ? Font.Medium : Font.Normal
                font.pointSize: 12
                color: Style.colorBaseText
            }
            
            
            MButton
            {
                id: closeButton
                Layout.preferredHeight: 26
                Layout.preferredWidth: 26
                Layout.rightMargin: 16
                Layout.alignment: Qt.AlignVCenter
                backgroundColor: "transparent"
                opacityOnPressed: 0.7
                borderColor: currentlyPressed ? Style.colorLightBorder : "transparent"
                radius: 4
                imagePath: currentlyPressed ? Icons.closePurple : Icons.closeGray
                imageSize: 12
                
                onClicked: root.removeTag(index)
            }
        }
    }
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
       
        onPressed: (mouse) => mouse.accepted = false
    }
}