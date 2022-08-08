import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import CustomComponents
import Librum.style
import Librum.icons


Popup
{
    id: root
    property int contentPadding: 16
    property MButton lastFocusedButton: applyButton
    
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
    
    onAboutToHide: applyButton.forceActiveFocus();
    
    Component.onCompleted: applyButton.forceActiveFocus()
    
    
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
                font.family: Style.defaultFontFamily
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
                        spacing: 0
                        
                        
                        Rectangle
                        {
                            id: bookCoverContainer
                            Layout.preferredWidth: parent.width
                            Layout.preferredHeight: Layout.preferredWidth * 1.30
                            color: Style.colorLightBorder
                            radius: 4
                            
                            Image
                            {
                                id: bookCover
                                anchors.centerIn: parent
                                Layout.alignment: Qt.AlignHCenter
                                sourceSize.height: bookCoverContainer.height - 2
                                source: Icons.bookCover
                                fillMode: Image.PreserveAspectFit
                            }
                        }
                        
                        RowLayout
                        {
                            id: bookButtons
                            Layout.topMargin: 28
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
                                text: "Choose"
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
                                text: "Reset"
                                fontColor: Style.colorBaseText
                                fontWeight: Font.DemiBold
                                fontSize: 11.5
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
                        anchors.leftMargin: 26
                        contentWidth: width
                        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                        ScrollBar.vertical: ScrollBar
                        {
                            x: parent.width
                            y: parent.topPadding
                            height: parent.availableHeight
                            policy: ScrollBar.AlwaysOn
                            onActiveChanged: parent.scrolling()
                        }
                        
                        
                        ColumnLayout
                        {
                            id: inputLayout
                            anchors.fill: parent
                            anchors.rightMargin: 8
                            spacing: 15
                            
                            
                            MLabeledInputBox
                            {
                                id: titleField
                                Layout.fillWidth: true
                                Layout.topMargin: -3
                                boxHeight: 34
                                headerText: "Title"
                                headerFontWeight: Font.Bold
                                headerFontSize: 11.5
                                text: "The 7 habits of highly effective people"
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
                                headerText: "Authors"
                                headerFontWeight: Font.Bold
                                headerFontSize: 11.5
                                text: "Stephen R. Covey, Jeff N. Murray"
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
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
                                text: "431"
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
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
                                imagePath: Icons.dropdownGray
                                imageSize: 9
                                selectionPopup.maxHeight: 200
                                selectionPopup.model: ListModel
                                {
                                    ListElement { text: "Technology" }
                                    ListElement { text: "Favourite" }
                                    ListElement { text: "Romance" }
                                    ListElement { text: "Comedy" }
                                    ListElement { text: "Sports" }
                                    ListElement { text: "Physics" }
                                    ListElement { text: "Blockchain" }
                                    ListElement { text: "Psychology" }
                                }
                            }
                            
                            MComboBox
                            {
                                id: languageComboBox
                                Layout.fillWidth: true
                                Layout.preferredHeight: 53
                                multiSelect: true
                                headerText: "Language"
                                headerFontSize: 11.5
                                headerFontColor: Style.colorBaseTitle
                                imagePath: Icons.dropdownGray
                                imageSize: 9
                                selectionPopup.maxHeight: 200
                                
                                selectionPopup.model: ListModel
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
                                headerText: "Publication"
                                headerFontWeight: Font.Bold
                                headerFontSize: 11.5
                                text: "United States: Dodd, Mead and Company,1922."
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
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
                                text: "24.02.2006"
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
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
                                text: "PDF"
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
                                borderWidth: 1
                                borderRadius: 4
                                readOnly: true
                            }
                            
                            MLabeledInputBox
                            {
                                id: sizeField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: "Size"
                                headerFontWeight: Font.Bold
                                headerFontSize: 11.5
                                text: "124 MB"
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
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
                                text: "20.08.2021"
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
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
                                text: "4 days ago"
                                headerToBoxSpacing: 3
                                inputFontSize: 12
                                inputFontColor: Style.colorLightText3
                                borderWidth: 1
                                borderRadius: 4
                                readOnly: true
                            }
                        }
                        
                        
                        Component.onCompleted: contentItem.maximumFlickVelocity = 600
                        
                        
                        function scrolling()
                        {
                            languageComboBox.selectionPopup.close();
                            tagsComboBox.selectionPopup.close();
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
                    active: true
                    borderWidth: activeFocus || currentlyHovered ? 0 : 1
                    borderColor: Style.colorLightBorder
                    backgroundColor: activeFocus || currentlyHovered ? Style.colorBasePurple : "transparent"
                    text: "Apply"
                    fontColor: activeFocus || currentlyHovered ? Style.colorBrightText : Style.colorLightText2
                    fontWeight: Font.Bold
                    fontSize: 12
                    
                    onActiveFocusChanged: if(activeFocus) root.lastFocusedButton = this
                    onClicked: root.close()
                    
                    Keys.onReturnPressed: root.close();
                    
                    KeyNavigation.right: cancelButton
                    KeyNavigation.tab: cancelButton
                }
                
                MButton
                {
                    id: cancelButton
                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignLeft
                    borderWidth: activeFocus  || currentlyHovered ? 0 : 1
                    borderColor: Style.colorLightBorder
                    backgroundColor: activeFocus || currentlyHovered ? Style.colorBasePurple : "transparent"
                    opacityOnPressed: 0.7
                    text: "Cancel"
                    fontColor: activeFocus || currentlyHovered ? Style.colorBrightText : Style.colorLightText2
                    fontWeight: Font.Bold
                    fontSize: 12
                    
                    onActiveFocusChanged: if(activeFocus) root.lastFocusedButton = this
                    onClicked: root.close()
                    
                    Keys.onReturnPressed: root.close();
                    
                    KeyNavigation.left: applyButton
                    KeyNavigation.right: deleteButton
                    KeyNavigation.tab: deleteButton
                }
                
                Item { Layout.fillWidth: true }
                
                MButton
                {
                    id: deleteButton
                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignRight
                    borderWidth: activeFocus  || currentlyHovered ? 0 : 1
                    borderColor: Style.colorLightBorder
                    backgroundColor: activeFocus || currentlyHovered ? Style.colorBaseRed : "transparent"
                    opacityOnPressed: 0.7
                    text: "Delete"
                    fontColor: activeFocus || currentlyHovered ? Style.colorBrightText : Style.colorLightText
                    fontWeight: Font.Bold
                    fontSize: 12
                    imagePath: activeFocus || currentlyHovered ? Icons.trashWhite : Icons.trashGray
                    imageSize: 17
                    imageSpacing: 10
                    
                    onActiveFocusChanged: if(activeFocus) root.lastFocusedButton = this
                    onClicked: acceptDeletionPopup.open();
                    
                    Keys.onReturnPressed: acceptDeletionPopup.open();
                    
                    KeyNavigation.left: cancelButton
                    KeyNavigation.tab: applyButton
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
        acceptLabel: "Choose"
        fileMode: FileDialog.OpenFile
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        nameFilters: ["png files (*.png)", "jpg files (*.jpg)", "jpeg files (*.jpeg)"]
    }
}