import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons


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
            source: Icons.dropupLightGray
        }
        
        Pane
        {
            id: container
            Layout.fillWidth: true
            padding: 14
            background: Rectangle
            {
                color: Style.colorBackground
                border.width: 1
                border.color: Style.colorLightBorder
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
                    placeholderColor: Style.colorLightText
                    headerText: "Author"
                    headerFontSize: 10.5
                    headerFontWeight: Font.Bold
                    headerFontColor: Style.colorLightText3
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
                        placeholderColor: Style.colorLightText
                        headerText: "From"
                        headerFontSize: 10.5
                        headerFontWeight: Font.Bold
                        headerFontColor: Style.colorLightText3
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
                        placeholderColor: Style.colorLightText
                        headerText: "To"
                        headerFontSize: 10.5
                        headerFontWeight: Font.Bold
                        headerFontColor: Style.colorLightText3
                        textPadding: 8
                        borderWidth: 1
                        borderRadius: 4
                    }
                }
                
                MComboBox
                {
                    id: languagesComboBox
                    Layout.fillWidth: true
                    Layout.preferredHeight: 49
                    headerText: "Language"
                    imagePath: Icons.dropdownGray
                    imageSize: 9
                    maxPopupHeight: 208
                    
                    listContent: ListModel
                    {
                        id: listModel
                        ListElement { content: "None" }
                        ListElement { content: "English" }
                        ListElement { content: "German"  }
                        ListElement { content: "Italian" }
                        ListElement { content: "French" }
                        ListElement { content: "Romanian" }
                        ListElement { content: "Spanish" }
                        ListElement { content: "Mandarin" }
                        ListElement { content: "Portugese" }
                        ListElement { content: "Hindi" }
                        ListElement { content: "Bengali" }
                        ListElement { content: "Russian" }
                        ListElement { content: "Arabic" }
                        ListElement { content: "Japanese" }
                        ListElement { content: "Indonesian" }
                        ListElement { content: "Turkish" }
                        ListElement { content: "Korean" }
                        ListElement { content: "Hungarian" }
                        ListElement { content: "Thai"  }
                        ListElement { content: "Swahli" }
                        ListElement { content: "Dutch" }
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
                    backgroundColor: Style.colorBasePurple
                    text: "Apply Filter"
                    fontSize: 10.5
                    fontColor: Style.colorBrightText
                    fontWeight: Font.Bold
                    
                    onClicked: root.filterQuerySent()
                }
            }
        }
    }
}