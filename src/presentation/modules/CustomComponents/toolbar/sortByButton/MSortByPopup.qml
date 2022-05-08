import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Popup
{
    id: root
    padding: 0
    implicitHeight: 183
    implicitWidth: 168
    background: Rectangle
    {
        color: "transparent"
    }
    
    
    ColumnLayout
    {
        id: mainLayout
        anchors.fill: parent
        spacing: 0
        
        
        Image
        {
            Layout.leftMargin: 14
            Layout.bottomMargin: -1
            source: properties.iconArrowUpFilled
        }
        
        Pane
        {
            id: container
            Layout.fillHeight: true
            Layout.fillWidth: true
            verticalPadding: 10
            horizontalPadding: 8
            background: Rectangle
            {
                color: properties.colorBackground
                border.width: 1
                border.color: properties.colorLightBorder
                radius: 6
                antialiasing: true
            }
            
            
            ColumnLayout
            {
                id: itemLayout
                property MSortByItem currentSelected : null
                
                width: parent.width
                spacing: 2
                
                
                MSortByItem
                {
                    Layout.fillWidth: true
                    selected: true
                    text: "Recently read"
                    onClicked: itemLayout.changeSelected(this)
                    
                    Component.onCompleted: itemLayout.currentSelected = this
                }
                
                MSortByItem
                {
                    Layout.fillWidth: true
                    selected: false
                    text: "Recently added"
                    onClicked: itemLayout.changeSelected(this)
                }
                
                MSortByItem
                {
                    Layout.fillWidth: true
                    selected: false
                    text: "By Percentage"
                    onClicked: itemLayout.changeSelected(this)
                }
                
                MSortByItem
                {
                    Layout.fillWidth: true
                    selected: false
                    text: "Book (A-Z)"
                    onClicked: itemLayout.changeSelected(this)
                }
                
                MSortByItem
                {
                    Layout.fillWidth: true
                    selected: false
                    text: "Author (A-Z)"
                    onClicked: itemLayout.changeSelected(this)
                }
                
                function changeSelected(newSelected)
                {
                    itemLayout.currentSelected.selected = false;
                    itemLayout.currentSelected = newSelected;
                    itemLayout.currentSelected.selected = true;
                }
            }
        }
    }
}