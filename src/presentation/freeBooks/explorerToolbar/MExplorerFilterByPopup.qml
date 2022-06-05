import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents

Popup
{
    id: root
    signal filterQuerySent
    
    padding: 0
    implicitWidth: 273
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
            padding: 14
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
                width: parent.width
                spacing: 11
                
                
                MLabeledInputBox
                {
                    id: authorInput
                    Layout.fillWidth: true
                    boxHeight: 30
                    headerToBoxSpacing: 2
                    placeholderContent: "e.g. Uncle bob"
                    inputFontSize: 11
                    placeholderColor: properties.colorLightText
                    headerText: "Author"
                    headerFontSize: 10.5
                    headerFontWeight: Font.Bold
                    headerFontColor: properties.colorLightText3
                    textPadding: 8
                    borderWidth: 1
                    borderRadius: 4
                }
                
                RowLayout
                {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 17
                    
                    
                    MLabeledInputBox
                    {
                        id: addedInput
                        Layout.fillWidth: true
                        boxHeight: 30
                        headerToBoxSpacing: 2
                        placeholderContent: "Year from"
                        inputFontSize: 11
                        placeholderColor: properties.colorLightText
                        headerText: "From"
                        headerFontSize: 10.5
                        headerFontWeight: Font.Bold
                        headerFontColor: properties.colorLightText3
                        textPadding: 8
                        borderWidth: 1
                        borderRadius: 4
                    }
                    
                    MLabeledInputBox
                    {
                        id: formatInput
                        Layout.fillWidth: true
                        boxHeight: 30
                        headerToBoxSpacing: 2
                        placeholderContent: "Year to"
                        inputFontSize: 11
                        placeholderColor: properties.colorLightText
                        headerText: "To"
                        headerFontSize: 10.5
                        headerFontWeight: Font.Bold
                        headerFontColor: properties.colorLightText3
                        textPadding: 8
                        borderWidth: 1
                        borderRadius: 4
                    }
                }
                
                MButton
                {
                    id: applyButton
                    Layout.fillWidth: true
                    Layout.preferredHeight: 28
                    Layout.topMargin: 8
                    buttonRadius: 3
                    borderWidth: 0
                    backgroundColor: properties.colorBasePurple
                    textContent: "Apply Filter"
                    fontSize: 10.5
                    fontColor: properties.colorBrightText
                    fontBold: true
                    
                    onClicked: root.filterQuerySent()
                }
            }
        }
    }
}