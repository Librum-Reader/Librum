import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0


Popup
{
    id: root
    signal filterQuerySent
    
    padding: 0
    implicitWidth: 273
    implicitHeight: mainLayout.height
    background: Rectangle
    {
        color: "transparent"
    }
    
    
    MFlickWrapper
    {
        anchors.fill: parent
        contentHeight: mainLayout.height
        
        ColumnLayout
        {
            id: mainLayout
            property int popUpTopSpacing: 6        
            
            width: parent.width
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
                        headerText: "Authors"
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
                        multiSelect: true
                        headerText: "Language"
                        image: Icons.dropdownGray
                        imageSize: 9
                        
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
                        fontColor: Style.colorBrightText
                        fontWeight: Font.Bold
                        
                        onClicked: root.filterQuerySent()
                    }
                }
            }
        }
    }
}