import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents

Popup
{
    id: root
    padding: 0
    implicitWidth: 151
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
            source: properties.iconDropupLightGray
        }
        
        Pane
        {
            id: container
            Layout.fillHeight: true
            Layout.fillWidth: true
            verticalPadding: 8
            horizontalPadding: 4
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
                property MDropDownListItem currentSelected : null
                
                width: parent.width
                spacing: 0
                
                
                MDropDownListItem
                {
                    Layout.fillWidth: true
                    selected: true
                    text: "Recently read"
                    onClicked: itemLayout.changeSelected(this)
                    
                    Component.onCompleted: itemLayout.currentSelected = this
                }
                
                MDropDownListItem
                {
                    Layout.fillWidth: true
                    selected: false
                    text: "Recently added"
                    onClicked: itemLayout.changeSelected(this)
                }
                
                MDropDownListItem
                {
                    Layout.fillWidth: true
                    selected: false
                    text: "By Percentage"
                    onClicked: itemLayout.changeSelected(this)
                }
                
                MDropDownListItem
                {
                    Layout.fillWidth: true
                    selected: false
                    text: "Book (A-Z)"
                    onClicked: itemLayout.changeSelected(this)
                }
                
                MDropDownListItem
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