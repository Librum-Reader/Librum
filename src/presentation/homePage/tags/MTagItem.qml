import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons


Item
{
    id: root
    required property int index
    required property string text
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
                text: root.text
                font.weight: root.selected ? Font.Medium : Font.Normal
                font.pointSize: 12
                font.family: Style.defaultFontFamily
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
                
                onClicked: root.removeTag(root.index)
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