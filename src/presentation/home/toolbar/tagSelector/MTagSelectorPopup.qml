import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents

Popup
{
    id: root
    signal tagSelctionMade
    
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
            source: properties.iconArrowUpFilled
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
                    text: "Technology"
                    onClicked: itemLayout.changeSelected(this)
                }
                
                MDropDownListItem
                {
                    Layout.fillWidth: true
                    text: "To read"
                    onClicked: itemLayout.changeSelected(this)
                }
                
                MDropDownListItem
                {
                    Layout.fillWidth: true
                    text: "Romance"
                    onClicked: itemLayout.changeSelected(this)
                }
                
                function changeSelected(newSelected)
                {
                    if(itemLayout.currentSelected != null)
                    {
                        itemLayout.currentSelected.selected = false;
                    }
                    itemLayout.currentSelected = newSelected;
                    itemLayout.currentSelected.selected = true;
                    
                    root.tagSelctionMade();
                }
            }
        }
    }
}