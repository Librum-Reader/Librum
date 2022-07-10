import QtQuick
import QtQuick.Controls
import CustomComponents
import QtQuick.Layouts
import Librum.style


Item
{
    id: root
    required property ListView containingListview
    property bool selected: false
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
            color: root.selected ? Style.colorSidebarMark : 
                                    mouseArea.containsMouse || containingListview.currentIndex == index
                                        ? Style.colorLightGray : Style.colorBackground
            radius: 4
            antialiasing: true
        }
        
        RowLayout
        {
            anchors.fill: parent
            anchors.leftMargin: 2
            spacing: 9
            
            
            MCheckBox
            {
                id: checkBox
                Layout.preferredWidth: 18
                Layout.preferredHeight: 18
                checked: root.selected
                imageSize: 8
            }
            
            Label
            {
                id: content
                horizontalAlignment: Text.AlignLeft
                Layout.fillWidth: true
                color: Style.colorLightText3
                text: root.text
                font.pointSize: 10.75
                font.family: Style.defaultFontFamily
                font.weight: root.selected ? Font.Medium : Font.Normal
                elide: Text.ElideRight
            }
        }
    }
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        
        onClicked: root.clicked(root.index);
    }
}