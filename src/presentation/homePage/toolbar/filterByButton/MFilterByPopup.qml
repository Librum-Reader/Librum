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
                                
                                onCheckedChanged: if(checked && unreadBox.checked) unreadBox.checked = false
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
                                
                                onCheckedChanged: if(checked && readBox.checked) readBox.checked = false
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
                                titleEmptyText: "Any"
                                imagePath: Icons.dropdownGray
                                imageSize: 9
                                
                                selectionPopup.checkBoxStyle: false
                                selectionPopup.checkBoxSize: 17
                                selectionPopup.checkBoxImageSize: 9
                                selectionPopup.itemHeight: 29
                                selectionPopup.fontSize: 11
                                selectionPopup.model: ListModel
                                {
                                    ListElement { text: "Pdf" }
                                    ListElement { text: "Epub"  }
                                    ListElement { text: "Mobi" }
                                    ListElement { text: "Txt" }
                                }
                                
                                onItemChanged: selectionPopup.close()
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
                            
                                onCheckedChanged: if(checked && onlyFilesBox.checked) onlyFilesBox.checked = false
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
                                
                                onCheckedChanged: if(checked && onlyBooksBox.checked) onlyBooksBox.checked = false
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
}