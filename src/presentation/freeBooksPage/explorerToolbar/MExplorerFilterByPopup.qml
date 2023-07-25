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
    implicitHeight: layout.height
    background: Rectangle { color: "transparent" }
    
    
    MFlickWrapper
    {
        anchors.fill: parent
        contentHeight: layout.height
        
        
        ColumnLayout
        {
            id: layout
            width: parent.width
            spacing: 0
            
            
            Image
            {
                id: topTriangleDecoration
                Layout.leftMargin: 14
                Layout.bottomMargin: -1
                source: Icons.popupDroplet
            }
            
            Pane
            {
                id: container
                Layout.fillWidth: true
                padding: 14
                background: Rectangle
                {
                    color: Style.colorPopupBackground
                    border.width: 1
                    border.color: Style.colorContainerBorder
                    radius: 6
                    antialiasing: true
                }
                
                
                ColumnLayout
                {
                    id: inputLayout
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
                        placeholderColor: Style.colorPlaceholderText
                        headerText: "Authors"
                        headerFontSize: 10.5
                        headerFontWeight: Font.Bold
                        headerFontColor: Style.colorLightText
                        textPadding: 8
                        borderWidth: 1
                        borderRadius: 4
                    }
                    
                    RowLayout
                    {
                        id: timeRange
                        Layout.fillWidth: true
                        spacing: 17
                        
                        
                        MLabeledInputBox
                        {
                            id: yearFromInput
                            Layout.fillWidth: true
                            boxHeight: 30
                            headerToBoxSpacing: 2
                            placeholderContent: "Year from"
                            inputFontSize: 11
                            placeholderColor: Style.colorPlaceholderText
                            headerText: "From"
                            headerFontSize: 10.5
                            headerFontWeight: Font.Bold
                            headerFontColor: Style.colorLightText
                            textPadding: 8
                            borderWidth: 1
                            borderRadius: 4
                        }
                        
                        MLabeledInputBox
                        {
                            id: yearToInput
                            Layout.fillWidth: true
                            boxHeight: 30
                            headerToBoxSpacing: 2
                            placeholderContent: "Year to"
                            inputFontSize: 11
                            placeholderColor: Style.colorPlaceholderText
                            headerText: "To"
                            headerFontSize: 10.5
                            headerFontWeight: Font.Bold
                            headerFontColor: Style.colorLightText
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
                        multiSelect: true
                        headerText: "Language"
                        dropdownIconSize: 9
                        
                        model: ListModel
                        {
                            ListElement { text: "English" }
                            ListElement { text: "German"  }
                            ListElement { text: "Italian" }
                            ListElement { text: "French" }
                            ListElement { text: "Romanian" }
                            ListElement { text: "Spanish" }
                            ListElement { text: "Mandarin" }
                            ListElement { text: "Portugese" }
                            ListElement { text: "Hindi" }
                            ListElement { text: "Bengali" }
                            ListElement { text: "Russian" }
                            ListElement { text: "Arabic" }
                            ListElement { text: "Japanese" }
                            ListElement { text: "Indonesian" }
                            ListElement { text: "Turkish" }
                            ListElement { text: "Korean" }
                            ListElement { text: "Hungarian" }
                            ListElement { text: "Thai"  }
                            ListElement { text: "Swahli" }
                            ListElement { text: "Dutch" }
                        }
                    }
                    
                    MButton
                    {
                        id: applyButton
                        Layout.fillWidth: true
                        Layout.preferredHeight: 28
                        Layout.topMargin: 8
                        radius: 3
                        borderWidth: 0
                        backgroundColor: Style.colorBasePurple
                        text: "Apply Filter"
                        fontSize: 10.5
                        textColor: Style.colorFocusedButtonText
                        fontWeight: Font.Bold
                        
                        onClicked: root.filterQuerySent()
                    }
                }
            }
        }
    }
}