import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.0
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.controllers 1.0
import Librum.globals 1.0


Popup
{
    id: root
    property int contentPadding: 16
    
    implicitWidth: 751
    implicitHeight: layout.implicitHeight
    focus: true
    padding: 0
    background: Rectangle
    {
        radius: 6
        color: Style.colorBackground
    }
    modal: true
    Overlay.modal: Rectangle
    {
        color: "#aa32324D"
        opacity: 1
    }
    
    onAboutToShow: 
    { 
        applyButton.forceActiveFocus(); 
        applyButton.active = true;
        cancelButton.active = false;
        deleteButton.active = false;
        
        inputSideLayout.contentItem.contentY = 0;
        
        bookCover.source = Qt.binding( function () { return Globals.selectedBook.cover })
    }
    Component.onCompleted: { applyButton.forceActiveFocus(); applyButton.active = true }
    
    
    MouseArea
    {
        anchors.fill: parent
        
        propagateComposedEvents: true
        onClicked: lastFocusedButton.forceActiveFocus()
    }
    
    
    MFlickWrapper
    {
        anchors.fill: parent
        contentHeight: layout.height
        
        
        ColumnLayout
        {
            id: layout
            width: parent.width
            spacing: 0
            
            
            MButton
            {
                id: closeButton
                Layout.preferredHeight: 32
                Layout.preferredWidth: 32
                Layout.topMargin: 12
                Layout.rightMargin: 14
                Layout.alignment: Qt.AlignTop | Qt.AlignRight
                backgroundColor: "transparent"
                opacityOnPressed: 0.7
                borderColor: "transparent"
                radius: 6
                borderColorOnPressed: Style.colorLightBorder
                imagePath: Icons.closeBlack
                imageSize: 14
                
                onClicked: root.close()
            }
            
            Label
            {
                id: title
                Layout.topMargin: 20
                Layout.leftMargin: 52
                text: "Book details"
                font.weight: Font.Bold
                font.pointSize: 17
                color: Style.colorBaseTitle
            }
            
            SplitView
            {
                id: splitView
                Layout.fillWidth: true
                Layout.preferredHeight: 320
                Layout.topMargin: 36
                Layout.leftMargin: 52
                Layout.rightMargin: 52
                orientation: Qt.Horizontal
                spacing: 10
                smooth: true
                handle: RowLayout
                {
                    width: 9
                    spacing: 0
                    
                    
                    Rectangle
                    {
                        Layout.preferredWidth: 4
                        Layout.fillHeight: true
                        color: "transparent"
                    }
                    
                    Rectangle
                    {
                        Layout.preferredWidth: 1
                        Layout.fillHeight: true
                        color: Style.colorLightBorder
                    }
                    
                    Rectangle
                    {
                        Layout.preferredWidth: 4
                        Layout.fillHeight: true
                        color: "transparent"
                    }
                }
                
                
                Item
                {
                    id: bookCoverSide
                    SplitView.preferredWidth: 218
                    SplitView.minimumWidth: 80
                    SplitView.maximumWidth: 246
                    
                    ColumnLayout
                    {
                        id: bookCoverSideLayout
                        width: parent.width - 20
                        anchors.verticalCenter: parent.verticalCenter
                        clip: true
                        spacing: 0
                        
                        
                        Rectangle
                        {
                            id: bookCoverContainer
                            Layout.preferredWidth: parent.width
                            Layout.preferredHeight: 240
                            Layout.topMargin: 4
                            color: Style.colorLightBorder
                            radius: 4
                            
                            Image
                            {
                                id: bookCover
                                visible: Globals.selectedBook !== null ? source != "" : false
                                anchors.centerIn: parent
                                sourceSize.width: 188
                                sourceSize.height: 238
                            }
                            
                            Label
                            {
                                id: noImageLabel
                                anchors.centerIn: parent
                                visible: !bookCover.visible
                                color: Style.colorDarkGray
                                text: Globals.selectedBook !== null ? "." + Globals.selectedBook.format : ""
                                font.pointSize: 20
                                font.bold: true
                            }
                        }
                        
                        RowLayout
                        {
                            id: bookButtons
                            Layout.topMargin: 22
                            spacing: 14
                            
                            
                            MButton
                            {
                                id: changeButton
                                Layout.fillWidth: true
                                Layout.preferredHeight: 34
                                borderWidth: 1
                                borderColor: Style.colorLightBorder
                                backgroundColor: "transparent"
                                opacityOnPressed: 0.8
                                text: "Change"
                                fontColor: Style.colorBaseText
                                fontWeight: Font.DemiBold
                                fontSize: 11.5
                                
                                onClicked: chooseImageDialog.open()
                            }
                            
                            MButton
                            {
                                id: resetButton
                                Layout.fillWidth: true
                                Layout.preferredHeight: 34
                                borderWidth: 1
                                borderColor: Style.colorLightBorder
                                backgroundColor: "transparent"
                                opacityOnPressed: 0.8
                                text: "Delete"
                                fontColor: Style.colorBaseText
                                fontWeight: Font.DemiBold
                                fontSize: 11.5
                                
                                onClicked: bookCover.source = "";
                            }
                        }
                    }
                }
                
                Item
                {
                    id: inputSide
                    SplitView.minimumWidth: 100
                    SplitView.fillWidth: true
                    
                    ScrollView
                    {
                        id: inputSideLayout
                        anchors.fill: parent
                        anchors.topMargin: -5
                        anchors.rightMargin: -10
                        anchors.leftMargin: 26
                        contentWidth: width
                        clip: true
                        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                        
                        Component.onCompleted: contentItem.maximumFlickVelocity = 600
                        
                        
                        ColumnLayout
                        {
                            id: inputLayout
                            property string defaultText: "Unknown"
                            
                            width: parent.width - 18
                            height: parent.height
                            anchors.rightMargin: 8
                            spacing: 15
                            
                            
                            MLabeledInputBox
                            {
                                id: titleField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: "Title"
                                headerFontWeight: Font.Bold
                                headerFontSize: 11.5
                                text: Globals.selectedBook !== null && Globals.selectedBook.title !== "" ? 
                                          Globals.selectedBook.title : inputLayout.defaultText
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                            }
                            
                            MLabeledInputBox
                            {
                                id: authorField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: "Author"
                                headerFontWeight: Font.Bold
                                headerFontSize: 11.5
                                text: Globals.selectedBook !== null &&  Globals.selectedBook.author !== "" ? 
                                          Globals.selectedBook.author : inputLayout.defaultText
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                            }
                            
                            MLabeledInputBox
                            {
                                id: pagesField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: "Pages"
                                headerFontWeight: Font.Bold
                                headerFontSize: 11.5
                                text: Globals.selectedBook !== null ? 
                                          Globals.selectedBook.pageCount : inputLayout.defaultText
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                                readOnly: true
                            }
                            
                            MComboBox
                            {
                                id: tagsComboBox
                                Layout.fillWidth: true
                                Layout.preferredHeight: 53
                                multiSelect: true
                                headerText: "Tags"
                                titleEmptyText: "None"
                                headerFontSize: 11.5
                                headerFontColor: Style.colorBaseTitle
                                image: Icons.dropdownGray
                                imageSize: 9
                                maxHeight: 200
                                model: Globals.bookTags
                            }
                            
                            MComboBox
                            {
                                id: languageComboBox
                                Layout.fillWidth: true
                                Layout.preferredHeight: 53
                                multiSelect: false
                                headerText: "Language"
                                headerFontSize: 11.5
                                headerFontColor: Style.colorBaseTitle
                                image: Icons.dropdownGray
                                imageSize: 9
                                maxHeight: 200
                                
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
                            
                            MLabeledInputBox
                            {
                                id: publicationField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: "Document creator"
                                headerFontWeight: Font.Bold
                                headerFontSize: 11.5
                                text: Globals.selectedBook !== null &&  Globals.selectedBook.creator !== "" ? 
                                          Globals.selectedBook.creator : inputLayout.defaultText
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                            }
                            
                            MLabeledInputBox
                            {
                                id: releaseDateField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: "Release date"
                                headerFontWeight: Font.Bold
                                headerFontSize: 11.5
                                text: Globals.selectedBook !== null &&  Globals.selectedBook.creationDate !== "" ? 
                                          Globals.selectedBook.creationDate : inputLayout.defaultText
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                            }
                            
                            MLabeledInputBox
                            {
                                id: formatField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: "Format"
                                headerFontWeight: Font.Bold
                                headerFontSize: 11.5
                                text: Globals.selectedBook !== null &&  Globals.selectedBook.format !== "" ? 
                                          Globals.selectedBook.format : inputLayout.defaultText
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                                readOnly: true
                            }
                            
                            MLabeledInputBox
                            {
                                id: sizeField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: "Document size"
                                headerFontWeight: Font.Bold
                                headerFontSize: 11.5
                                text: Globals.selectedBook !== null &&  Globals.selectedBook.documentSize !== "" ? 
                                          Globals.selectedBook.documentSize : inputLayout.defaultText
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                                readOnly: true
                            }
                            
                            MLabeledInputBox
                            {
                                id: addedField                                
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: "Added"
                                headerFontWeight: Font.Bold
                                headerFontSize: 11.5
                                text: Globals.selectedBook !== null ? 
                                          Globals.selectedBook.addedToLibrary : inputLayout.defaultText
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                                readOnly: true
                            }
                            
                            MLabeledInputBox
                            {
                                id: lastOpenedField
                                Layout.fillWidth: true
                                Layout.bottomMargin: 3
                                boxHeight: 34
                                headerText: "Last opened"
                                headerFontWeight: Font.Bold
                                headerFontSize: 11.5
                                text: Globals.selectedBook !== null ? 
                                          Globals.selectedBook.lastOpened : inputLayout.defaultText
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                                readOnly: true
                            }
                        }
                    }
                }
            }              
            
            RowLayout
            {
                id: buttonLayout
                Layout.preferredWidth: parent.width
                Layout.topMargin: 65
                Layout.bottomMargin: 42
                Layout.leftMargin: 52
                Layout.rightMargin: 52
                spacing: 16
                
                
                MButton
                {
                    id: applyButton
                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignLeft
                    borderWidth: active ? 0 : 1
                    borderColor: Style.colorLightBorder
                    backgroundColor: active ? Style.colorBasePurple : "transparent"
                    text: "Apply"
                    fontColor: active ? Style.colorBrightText : Style.colorLightText2
                    fontWeight: Font.Bold
                    fontSize: 12
                    
                    onClicked:
                    {
                        root.saveData();
                        root.close();
                    }
                    
                    Keys.onReturnPressed:
                    {
                        root.saveData();
                        root.close();
                    }
                    
                    Keys.onRightPressed: 
                    {
                        active = false;
                        cancelButton.active = true;
                        cancelButton.forceActiveFocus();
                    }
                    
                    Keys.onTabPressed: 
                    {
                        active = false;
                        cancelButton.active = true;
                        cancelButton.forceActiveFocus();
                    }
                }
                
                MButton
                {
                    id: cancelButton
                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignLeft
                    borderWidth: active ? 0 : 1
                    borderColor: Style.colorLightBorder
                    backgroundColor: active ? Style.colorBasePurple : "transparent"
                    opacityOnPressed: 0.7
                    text: "Cancel"
                    fontColor: active ? Style.colorBrightText : Style.colorLightText2
                    fontWeight: Font.Bold
                    fontSize: 12
                    
                    onClicked: root.close()
                    Keys.onReturnPressed: root.close();
                    
                    Keys.onLeftPressed: 
                    {
                        active = false;
                        applyButton.active = true;
                        applyButton.forceActiveFocus();
                    }
                    
                    Keys.onRightPressed: 
                    {
                        active = false;
                        deleteButton.active = true;
                        deleteButton.forceActiveFocus();
                    }
                    
                    Keys.onTabPressed: 
                    {
                        active = false;
                        deleteButton.active = true;
                        deleteButton.forceActiveFocus();
                    }
                }
                
                Item { Layout.fillWidth: true }
                
                MButton
                {
                    id: deleteButton
                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignRight
                    borderWidth: active ? 0 : 1
                    borderColor: Style.colorLightBorder
                    backgroundColor: active ? Style.colorBaseRed : "transparent"
                    opacityOnPressed: 0.7
                    text: "Delete"
                    fontColor: active ? Style.colorBrightText : Style.colorLightText
                    fontWeight: Font.Bold
                    fontSize: 12
                    imagePath: active ? Icons.trashWhite : Icons.trashGray
                    imageSize: 17
                    imageSpacing: 10
                    
                    onClicked: acceptDeletionPopup.open();
                    
                    Keys.onReturnPressed: acceptDeletionPopup.open();
                    
                    Keys.onLeftPressed: 
                    {
                        active = false;
                        cancelButton.active = true;
                        cancelButton.forceActiveFocus();
                    }
                    
                    Keys.onTabPressed: 
                    {
                        active = false;
                        applyButton.active = true;
                        applyButton.forceActiveFocus();
                    }
                }
            }
        }
    }
    
    
    MAcceptDeletionPopup
    {
        id: acceptDeletionPopup
        x: root.width / 2 - implicitWidth / 2
        y: root.height / 2 - implicitHeight / 2 - 30
        
        onDeleteChoosed: root.close()
    }
    
    FileDialog
    {
        id: chooseImageDialog
        acceptLabel: "Select"
        fileMode: FileDialog.OpenFile
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        nameFilters: ["All files (*.png *.jpg *.jpg *.jpeg)", "png files (*.png)", "jpg files (*.jpg)", 
            "jpeg files (*.jpeg)"]
        
        onAccepted: bookCover.source = file
    }
    
    
    function saveData()
    {
        var operationsMap = {};
        
        if(titleField.text !== Globals.selectedBook.title && titleField.text != inputLayout.defaultText)
            operationsMap[BookController.MetaProperties.Title] = titleField.text;
        
        if(authorField.text !== Globals.selectedBook.author && authorField.text != inputLayout.defaultText)
            operationsMap[BookController.MetaProperties.Author] = authorField.text;
        
        if(languageComboBox.text !== Globals.selectedBook.language && languageComboBox.text != "")
            operationsMap[BookController.MetaProperties.Language] = languageComboBox.text;
        
        if(publicationField.text !== Globals.selectedBook.creator && publicationField.text != inputLayout.defaultText)
            operationsMap[BookController.MetaProperties.Creator] = publicationField.text;
        
        if(releaseDateField.text !== Globals.selectedBook.creationDate && releaseDateField.text != inputLayout.defaultText)
            operationsMap[BookController.MetaProperties.CreationDate] = releaseDateField.text;
        
        if(formatField.text !== Globals.selectedBook.format && formatField.text != inputLayout.defaultText)
            operationsMap[BookController.MetaProperties.Format] = formatField.text;
        
        // @disable-check M126
        if(bookCover.source != Globals.selectedBook.cover)   // Needs to be !=, the types are different (QUrl and QString)
            operationsMap[BookController.MetaProperties.Cover] = bookCover.source;
        
        
        BookController.updateBook(Globals.selectedBook.title, operationsMap);
    }
}