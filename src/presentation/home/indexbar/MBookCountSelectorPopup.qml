import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents

Popup
{
    id: root
    signal bookCountSelctionMade(string content)
    
    padding: 0
    implicitWidth: 58
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
                property MDropDownListItem currentSelected : null
                
                width: parent.width
                spacing: 0
                
                
                MDropDownListItem
                {
                    Layout.fillWidth: true
                    text: "2"
                    onClicked: itemLayout.changeSelected(this)
                    padding: 12
                }
                
                MDropDownListItem
                {
                    Layout.fillWidth: true
                    text: "4"
                    onClicked: itemLayout.changeSelected(this)
                    padding: 12
                }
                
                MDropDownListItem
                {
                    Layout.fillWidth: true
                    text: "8"
                    onClicked: itemLayout.changeSelected(this)
                    padding: 12
                }
                
                MDropDownListItem
                {
                    Layout.fillWidth: true
                    text: "10"
                    onClicked: itemLayout.changeSelected(this)
                    padding: 8
                }
                
                MDropDownListItem
                {
                    Layout.fillWidth: true
                    text: "12"
                    onClicked: itemLayout.changeSelected(this)
                    padding: 8
                    
                    Component.onCompleted: itemLayout.changeSelected(this)
                }
                
                MDropDownListItem
                {
                    Layout.fillWidth: true
                    text: "14"
                    onClicked: itemLayout.changeSelected(this)
                    padding: 8
                }
                
                function changeSelected(newSelected)
                {
                    if(itemLayout.currentSelected != null)
                    {
                        itemLayout.currentSelected.selected = false;
                    }
                    itemLayout.currentSelected = newSelected;
                    itemLayout.currentSelected.selected = true;
                    
                    root.bookCountSelctionMade(itemLayout.currentSelected.text);
                }
            }
        }
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}