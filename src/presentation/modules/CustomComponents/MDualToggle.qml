import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style

Item
{
    id: root
    property string selected: "Light"
    
    implicitHeight: 40
    implicitWidth: 178
    
    
    Pane
    {
        id: container
        anchors.fill: parent
        padding: 0
        background: Rectangle
        {
            color: Style.colorBackground
            border.color: Style.colorLightBorder
            radius: 4
        }
        
        
        RowLayout
        {
            id: mainLayout
            anchors.fill: parent
            spacing: 0
            
            
            Label
            {
                id: darkText
                Layout.fillHeight: true
                Layout.preferredWidth: (root.width - separator.width) / 2
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: "Dark"
                color: root.selected == "Dark" ? Style.colorBasePurple : Style.colorLightText3
                font.pointSize: 12
                font.weight: root.selected == "Dark" ? Font.Bold : Font.DemiBold
                background: Rectangle
                {
                    anchors.fill: parent
                    anchors.margins: 1
                    color: root.selected == "Dark" ? Style.colorSidebarMark : "transparent"
                    radius: 4
                }
                
                Rectangle
                {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: 1
                    anchors.bottomMargin: 1
                    width: 3
                    color: root.selected == "Dark" ? Style.colorSidebarMark : "transparent"
                }
                
                MouseArea
                {
                    anchors.fill: parent
                    
                    onClicked: root.selected = "Dark"
                }
            }
            
            Rectangle
            {
                id: separator
                Layout.fillHeight: true
                Layout.preferredWidth: 2
                color: Style.colorMediumPurple
            }
            
            Label
            {
                id: lightText
                Layout.fillHeight: true
                Layout.preferredWidth: (root.width - separator.width) / 2
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: "Light"
                color: root.selected == "Light" ? Style.colorBasePurple : Style.colorLightText3
                font.pointSize: 12
                font.weight: root.selected == "Light" ? Font.Bold : Font.DemiBold
                background: Rectangle
                {
                    anchors.fill: parent
                    anchors.margins: 1
                    color: root.selected == "Light" ? Style.colorSidebarMark : "transparent"
                    radius: 4
                }
                
                Rectangle
                {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: 1
                    anchors.bottomMargin: 1
                    width: 3
                    color: root.selected == "Light" ? Style.colorSidebarMark : "transparent"
                }
                
                MouseArea
                {
                    anchors.fill: parent
                    
                    onClicked: root.selected = "Light"
                }
            }
        }
    }
}