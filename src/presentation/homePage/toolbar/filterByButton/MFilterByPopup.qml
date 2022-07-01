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
            Layout.fillHeight: true
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
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 17
                    
                    
                    ColumnLayout
                    {
                        Layout.fillWidth: true
                        
                        
                        MLabeledInputBox
                        {
                            id: addedInput
                            Layout.fillWidth: true
                            Layout.topMargin: 2
                            boxHeight: 30
                            headerToBoxSpacing: 2
                            placeholderContent: "e.g. 2d ago"
                            inputFontSize: 11
                            placeholderColor: Style.colorLightText
                            headerText: "Added"
                            headerFontSize: 10.5
                            headerFontWeight: Font.Bold
                            headerFontColor: Style.colorLightText3
                            textPadding: 8
                            borderWidth: 1
                            borderRadius: 4
                        }
                        
                        MLabeledCheckBox
                        {
                            id: readBox
                            Layout.topMargin: 16
                            Layout.fillWidth: true
                            boxWidth: 17
                            boxHeight: 17
                            borderColor: Style.colorLightBorder2
                            spacing:  7
                            text: "Read"
                            fontSize: 10.5
                            fontWeight: Font.Normal
                            fontColor: Style.colorLightText3
                            imageSize: 9
                        }
                        
                        MLabeledCheckBox
                        {
                            id: unreadBox
                            Layout.topMargin: 10
                            Layout.fillWidth: true
                            boxWidth: 17
                            boxHeight: 17
                            borderColor: Style.colorLightBorder2
                            spacing:  7
                            text: "Unread"
                            fontSize: 10.5
                            fontWeight: Font.Normal
                            fontColor: Style.colorLightText3
                            imageSize: 9
                        }
                    }
                    
                    ColumnLayout
                    {
                        Layout.fillWidth: true
                        
                        MComboBox
                        {
                            id: formatInputComboBox
                            Layout.fillWidth: true
                            Layout.preferredHeight: 49
                            Layout.topMargin: 2
                            headerText: "Format"
                            imagePath: Icons.dropdownGray
                            imageSize: 9
                            maxPopupHeight: 208
                            
                            listContent: listModel
                            
                            ListModel
                            {
                                id: listModel
                                ListElement { content: "Any" }
                                ListElement { content: "Pdf" }
                                ListElement { content: "Epub"  }
                                ListElement { content: "Mobi" }
                            }
                        }
                        
                        MLabeledCheckBox
                        {
                            id: onlyBooksBox
                            Layout.topMargin: 14
                            Layout.fillWidth: true
                            boxWidth: 17
                            boxHeight: 17
                            borderColor: Style.colorLightBorder2
                            spacing:  7
                            text: "Only Books"
                            fontSize: 10.5
                            fontWeight: Font.Normal
                            fontColor: Style.colorLightText3
                            imageSize: 9
                        }
                        
                        MLabeledCheckBox
                        {
                            id: onlyFilesBox
                            Layout.topMargin: 10
                            Layout.fillWidth: true
                            boxWidth: 17
                            boxHeight: 17
                            borderColor: Style.colorLightBorder2
                            spacing: 7
                            text: "Only Files"
                            fontSize: 10.5
                            fontWeight: Font.Normal
                            fontColor: Style.colorLightText3
                            imageSize: 9
                        }
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