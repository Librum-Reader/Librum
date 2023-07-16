import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons


Popup
{
    id: root
    property alias authors: authorsInput.text
    property alias format: formatInputComboBox.text
    property alias date: addedInput.text
    property alias onlyBooks: onlyBooksBox.checked
    property alias onlyFiles: onlyFilesBox.checked
    property alias read: readBox.checked
    property alias unread: unreadBox.checked
    signal filterQuerySent
    
    padding: 0
    implicitWidth: 273
    implicitHeight: layout.height
    background: Rectangle { color: "transparent" }
    
    
    Shortcut
    {
        sequence: "Return"
        enabled: root.opened
        onActivated: root.filterQuerySent()
    }
    
    
    MFlickWrapper
    {
        id: flickWrapper
        anchors.fill: parent
        contentHeight: layout.height
        
        ColumnLayout
        {
            id: layout
            width: parent.width
            spacing: 0
            
            
            Image
            {
                id: triangleDecoration
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
                    id: itemLayout
                    width: parent.width
                    spacing: 11
                    
                    
                    MLabeledInputBox
                    {
                        id: authorsInput
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
                        id: horizontalInputLayout
                        Layout.fillWidth: true
                        spacing: 17
                        
                        
                        ColumnLayout
                        {
                            id: leftColumnLayout
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
                                placeholderColor: Style.colorPlaceholderText
                                headerText: "Added"
                                headerFontSize: 10.5
                                headerFontWeight: Font.Bold
                                headerFontColor: Style.colorLightText
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
                                spacing: 7
                                text: "Read"
                                fontSize: 10.5
                                fontWeight: Font.Normal
                                fontColor: Style.colorLightText
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
                                spacing: 7
                                text: "Unread"
                                fontSize: 10.5
                                fontWeight: Font.Normal
                                fontColor: Style.colorLightText
                                imageSize: 9
                                
                                onCheckedChanged: if(checked && readBox.checked) readBox.checked = false
                            }
                        }
                        
                        ColumnLayout
                        {
                            id: rightColumnlayout
                            Layout.fillWidth: true
                            
                            MComboBox
                            {
                                id: formatInputComboBox
                                Layout.fillWidth: true
                                Layout.preferredHeight: 49
                                Layout.topMargin: 2
                                headerText: "Format"
                                emptyText: "Any"
                                dropdownIconSize: 9
                                
                                checkBoxStyle: false
                                checkBoxSize: 17
                                checkBoxImageSize: 9
                                itemHeight: 29
                                fontSize: 11
                                model: ListModel
                                {
                                    ListElement { text: "Pdf" }
                                    ListElement { text: "Epub"  }
                                    ListElement { text: "Mobi" }
                                    ListElement { text: "Txt" }
                                }
                                
                                onItemChanged: formatInputComboBox.closePopup()
                            }
                            
                            MLabeledCheckBox
                            {
                                id: onlyBooksBox
                                Layout.topMargin: 14
                                Layout.fillWidth: true
                                boxWidth: 17
                                boxHeight: 17
                                spacing: 7
                                text: "Only Books"
                                fontSize: 10.5
                                fontWeight: Font.Normal
                                fontColor: Style.colorLightText
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
                                spacing: 7
                                text: "Only Files"
                                fontSize: 10.5
                                fontWeight: Font.Normal
                                fontColor: Style.colorLightText
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
                        textColor: Style.colorFocusedButtonText
                        fontWeight: Font.Bold
                        
                        onClicked: root.filterQuerySent()
                    }
                }
            }
        }
    }
    
    function resetFilter()
    {
        authorsInput.clearText();
        addedInput.clearText();
        formatInputComboBox.deselectCurrenItem();
        unreadBox.checked = false;
        readBox.checked = false;
        onlyFilesBox.checked = false;
        onlyBooksBox.checked = false;
    }
}