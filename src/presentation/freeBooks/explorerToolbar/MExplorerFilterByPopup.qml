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
        property int popUpTopSpacing: 6        
        
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
                    id: timeRange
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 17
                    
                    
                    MLabeledInputBox
                    {
                        id: frominput
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
                        id: toInput
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
                
                MComboBox
                {
                    id: languagesComboBox
                    Layout.fillWidth: true
                    headerText: "Language"
                    imagePath: properties.iconArrowDownFilled
                    maxPopupHeight: 208
                    
                    listContent: listModel
                    
                    ListModel
                    {
                        id: listModel
                        ListElement { languageName: "None" }
                        ListElement { languageName: "English" }
                        ListElement { languageName: "German"  }
                        ListElement { languageName: "Italian" }
                        ListElement { languageName: "French" }
                        ListElement { languageName: "Romanian" }
                        ListElement { languageName: "Spanish" }
                        ListElement { languageName: "Mandarin" }
                        ListElement { languageName: "Portugese" }
                        ListElement { languageName: "Hindi" }
                        ListElement { languageName: "Bengali" }
                        ListElement { languageName: "Russian" }
                        ListElement { languageName: "Arabic" }
                        ListElement { languageName: "Japanese" }
                        ListElement { languageName: "Indonesian" }
                        ListElement { languageName: "Turkish" }
                        ListElement { languageName: "Korean" }
                        ListElement { languageName: "Hungarian" }
                        ListElement { languageName: "Thai"  }
                        ListElement { languageName: "Swahli" }
                        ListElement { languageName: "Dutch" }
                    }
                }
                
                MButton
                {
                    id: applyButton
                    Layout.fillWidth: true
                    Layout.preferredHeight: 28
                    Layout.topMargin: 10
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