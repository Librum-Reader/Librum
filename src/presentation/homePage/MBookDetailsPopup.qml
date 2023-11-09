import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import CustomComponents
import Librum.style
import Librum.icons
import Librum.controllers
import Librum.globals
import Librum.fonts

Popup {
    id: root
    implicitWidth: 751
    implicitHeight: layout.implicitHeight
    focus: true
    padding: 0
    background: Rectangle {
        radius: 6
        color: Style.colorPopupBackground
    }
    modal: true
    Overlay.modal: Rectangle {
        color: Style.colorPopupDim
        opacity: 1
    }

    onAboutToHide: internal.unloadData()
    onAboutToShow: {
        internal.setupPopup()
        internal.loadData()
    }
    Component.onCompleted: {
        applyButton.forceActiveFocus()
        applyButton.active = true
    }

    MFlickWrapper {
        id: flickWrapper
        anchors.fill: parent
        contentHeight: layout.height

        ColumnLayout {
            id: layout
            width: parent.width
            spacing: 0

            MButton {
                id: closeButton
                Layout.preferredHeight: 32
                Layout.preferredWidth: 32
                Layout.topMargin: 12
                Layout.rightMargin: 14
                Layout.alignment: Qt.AlignRight
                backgroundColor: "transparent"
                opacityOnPressed: 0.7
                borderColor: "transparent"
                radius: 6
                borderColorOnPressed: Style.colorButtonBorder
                imagePath: Icons.closePopup
                imageSize: 14

                onClicked: root.close()
            }

            Label {
                id: popupTitle
                Layout.topMargin: 20
                Layout.leftMargin: 52
                text: qsTr("Book details")
                font.weight: Font.Bold
                font.pointSize: Fonts.size17
                color: Style.colorTitle
            }


            /*
              The SplitView contains all the book information, it holds the book cover
              on the left side, and the book data on the right.
              */
            SplitView {
                id: splitView
                Layout.fillWidth: true
                Layout.preferredHeight: 320
                Layout.topMargin: 36
                Layout.leftMargin: 52
                Layout.rightMargin: 52
                orientation: Qt.Horizontal
                spacing: 10
                smooth: true
                // Create explicit handle to make the grabbable area bigger
                handle: RowLayout {
                    width: 9
                    spacing: 0

                    Rectangle {
                        Layout.preferredWidth: 4
                        Layout.fillHeight: true
                        color: "transparent"
                    }

                    Rectangle {
                        Layout.preferredWidth: 1
                        Layout.fillHeight: true
                        color: Style.colorDarkSeparator
                    }

                    Rectangle {
                        Layout.preferredWidth: 4
                        Layout.fillHeight: true
                        color: "transparent"
                    }
                }


                /*
                  The book cover side of the SplitView
                  */
                Item {
                    id: bookCoverSide
                    SplitView.preferredHeight: parent.height
                    SplitView.preferredWidth: 218
                    SplitView.minimumWidth: 80
                    SplitView.maximumWidth: 246

                    ColumnLayout {
                        id: bookCoverSideLayout
                        width: parent.width - 20
                        anchors.verticalCenter: parent.verticalCenter
                        clip: true
                        spacing: 0

                        Rectangle {
                            id: bookCoverContainer
                            Layout.preferredWidth: parent.width
                            Layout.preferredHeight: 240
                            Layout.topMargin: 4
                            color: Style.colorBookImageBackground
                            radius: 4

                            Image {
                                id: bookCover
                                visible: Globals.selectedBook !== null ? source != "" : false
                                anchors.centerIn: parent
                                sourceSize.width: 188
                                sourceSize.height: 238
                            }

                            Label {
                                id: noImageLabel
                                anchors.centerIn: parent
                                visible: !bookCover.visible
                                color: Style.colorNoImageLabel
                                text: Globals.selectedBook
                                      !== null ? "." + Globals.selectedBook.format : ""
                                font.pointSize: Fonts.size20
                                font.bold: true
                            }
                        }

                        RowLayout {
                            id: bookCoverButtons
                            Layout.topMargin: 22
                            spacing: 14

                            MButton {
                                id: changeButton
                                Layout.fillWidth: true
                                Layout.preferredHeight: 34
                                borderWidth: 1
                                backgroundColor: "transparent"
                                opacityOnPressed: 0.8
                                text: qsTr("Change")
                                textColor: Style.colorText
                                fontWeight: Font.DemiBold
                                fontSize: Fonts.size11dot5

                                onClicked: chooseImageDialog.open()
                            }

                            MButton {
                                id: resetButton
                                Layout.fillWidth: true
                                Layout.preferredHeight: 34
                                borderWidth: 1
                                backgroundColor: "transparent"
                                opacityOnPressed: 0.8
                                text: qsTr("Delete")
                                textColor: Style.colorText
                                fontWeight: Font.DemiBold
                                fontSize: Fonts.size11dot5

                                onClicked: bookCover.source = ""
                            }
                        }
                    }
                }


                /*
                  The book data side of the SplitView
                  */
                Item {
                    id: bookDataSide
                    SplitView.minimumWidth: 100
                    SplitView.fillWidth: true

                    ScrollView {
                        id: dataSideScrollView
                        anchors.fill: parent
                        anchors.topMargin: 0
                        anchors.rightMargin: -10
                        anchors.leftMargin: 26
                        contentWidth: width
                        clip: true
                        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

                        Component.onCompleted: {
                            contentItem.maximumFlickVelocity = 600
                            contentItem.boundsMovement = Flickable.StopAtBounds
                            contentItem.boundsBehavior = Flickable.StopAtBounds
                        }

                        ColumnLayout {
                            id: dataSideLayout
                            width: parent.width - 18
                            height: parent.height
                            anchors.rightMargin: 8
                            spacing: 13

                            MLabeledInputBox {
                                id: titleField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: qsTr("Title")
                                headerFontWeight: Font.Bold
                                headerFontSize: Fonts.size11dot5
                                text: Globals.selectedBook
                                      !== null ? Globals.selectedBook.title : ""
                                placeholderContent: qsTr("Unknown")
                                placeholderColor: Style.colorPlaceholderText
                                headerToBoxSpacing: 3
                                inputFontSize: Fonts.size12
                                inputFontColor: Style.colorLightInputText
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                            }

                            MLabeledInputBox {
                                id: authorsField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: qsTr("Authors")
                                headerFontWeight: Font.Bold
                                headerFontSize: Fonts.size11dot5
                                text: Globals.selectedBook
                                      !== null ? Globals.selectedBook.authors : ""
                                placeholderContent: qsTr("Unknown")
                                placeholderColor: Style.colorPlaceholderText
                                headerToBoxSpacing: 3
                                inputFontSize: Fonts.size12
                                inputFontColor: Style.colorLightInputText
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                            }

                            MLabeledInputBox {
                                id: pagesField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: qsTr("Pages")
                                headerFontWeight: Font.Bold
                                headerFontSize: Fonts.size11dot5
                                text: Globals.selectedBook !== null ? Globals.selectedBook.pageCount : internal.placeholderText
                                headerToBoxSpacing: 3
                                inputFontSize: Fonts.size12
                                inputFontColor: Style.colorLightInputText
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                                readOnly: true
                            }

                            MComboBox {
                                id: languageComboBox
                                Layout.fillWidth: true
                                Layout.preferredHeight: 53
                                multiSelect: false
                                headerText: qsTr("Language")
                                headerFontSize: Fonts.size11dot5
                                headerFontColor: Style.colorTitle
                                dropdownIconSize: 9
                                maxHeight: 200
                                model: ListModel {
                                    ListElement {
                                        text: "English"
                                    }
                                    ListElement {
                                        text: "German"
                                    }
                                    ListElement {
                                        text: "Italian"
                                    }
                                    ListElement {
                                        text: "French"
                                    }
                                    ListElement {
                                        text: "Romanian"
                                    }
                                    ListElement {
                                        text: "Spanish"
                                    }
                                    ListElement {
                                        text: "Mandarin"
                                    }
                                    ListElement {
                                        text: "Portugese"
                                    }
                                    ListElement {
                                        text: "Hindi"
                                    }
                                    ListElement {
                                        text: "Bengali"
                                    }
                                    ListElement {
                                        text: "Russian"
                                    }
                                    ListElement {
                                        text: "Arabic"
                                    }
                                    ListElement {
                                        text: "Japanese"
                                    }
                                    ListElement {
                                        text: "Indonesian"
                                    }
                                    ListElement {
                                        text: "Turkish"
                                    }
                                    ListElement {
                                        text: "Korean"
                                    }
                                    ListElement {
                                        text: "Hungarian"
                                    }
                                    ListElement {
                                        text: "Thai"
                                    }
                                    ListElement {
                                        text: "Swahli"
                                    }
                                    ListElement {
                                        text: "Dutch"
                                    }
                                }
                            }

                            MLabeledInputBox {
                                id: documentCreatorField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: qsTr("Document creator")
                                headerFontWeight: Font.Bold
                                headerFontSize: Fonts.size11dot5
                                text: Globals.selectedBook
                                      !== null ? Globals.selectedBook.creator : ""
                                placeholderContent: qsTr("Unknown")
                                placeholderColor: Style.colorPlaceholderText
                                headerToBoxSpacing: 3
                                inputFontSize: Fonts.size12
                                inputFontColor: Style.colorLightInputText
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                            }

                            MLabeledInputBox {
                                id: creationDateField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: qsTr("Creation date")
                                headerFontWeight: Font.Bold
                                headerFontSize: Fonts.size11dot5
                                text: Globals.selectedBook
                                      !== null ? Globals.selectedBook.creationDate : ""
                                placeholderContent: qsTr("Unknown")
                                placeholderColor: Style.colorPlaceholderText
                                headerToBoxSpacing: 3
                                inputFontSize: Fonts.size12
                                inputFontColor: Style.colorLightInputText
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                            }

                            MLabeledInputBox {
                                id: formatField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: qsTr("Format")
                                headerFontWeight: Font.Bold
                                headerFontSize: Fonts.size11dot5
                                text: Globals.selectedBook !== null
                                      && Globals.selectedBook.format !== "" ? Globals.selectedBook.format : internal.placeholderText
                                headerToBoxSpacing: 3
                                inputFontSize: Fonts.size12
                                inputFontColor: Style.colorLightInputText
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                                readOnly: true
                            }

                            MLabeledInputBox {
                                id: sizeField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: qsTr("Document size")
                                headerFontWeight: Font.Bold
                                headerFontSize: Fonts.size11dot5
                                text: Globals.selectedBook !== null
                                      && Globals.selectedBook.documentSize
                                      !== "" ? Globals.selectedBook.documentSize : internal.placeholderText
                                headerToBoxSpacing: 3
                                inputFontSize: Fonts.size12
                                inputFontColor: Style.colorLightInputText
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                                readOnly: true
                            }

                            MLabeledInputBox {
                                id: addedField
                                Layout.fillWidth: true
                                boxHeight: 34
                                headerText: qsTr("Added")
                                headerFontWeight: Font.Bold
                                headerFontSize: Fonts.size11dot5
                                text: Globals.selectedBook !== null ? Globals.selectedBook.addedToLibrary : internal.placeholderText
                                headerToBoxSpacing: 3
                                inputFontSize: Fonts.size12
                                inputFontColor: Style.colorLightInputText
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                                readOnly: true
                            }

                            MLabeledInputBox {
                                id: lastOpenedField
                                Layout.fillWidth: true
                                Layout.bottomMargin: 3
                                boxHeight: 34
                                headerText: qsTr("Last opened")
                                headerFontWeight: Font.Bold
                                headerFontSize: Fonts.size11dot5
                                text: Globals.selectedBook !== null ? Globals.selectedBook.lastOpened : internal.placeholderText

                                headerToBoxSpacing: 3
                                inputFontSize: Fonts.size12
                                inputFontColor: Style.colorLightInputText
                                textPadding: 12
                                borderWidth: 1
                                borderRadius: 4
                                readOnly: true
                            }
                        }
                    }
                }
            }

            RowLayout {
                id: buttonLayout
                Layout.preferredWidth: parent.width
                Layout.topMargin: 65
                Layout.bottomMargin: 42
                Layout.leftMargin: 52
                Layout.rightMargin: 52
                spacing: 16

                MButton {
                    id: applyButton
                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignLeft
                    borderWidth: active ? 0 : 1
                    backgroundColor: active ? Style.colorBasePurple : "transparent"
                    text: qsTr("Apply")
                    textColor: active ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                    fontWeight: Font.Bold
                    fontSize: Fonts.size12

                    onClicked: {
                        internal.saveData()
                        root.close()
                    }
                    Keys.onReturnPressed: {
                        internal.saveData()
                        root.close()
                    }
                    Keys.onRightPressed: internal.focusCancelButton()
                    Keys.onTabPressed: internal.focusCancelButton()
                }

                MButton {
                    id: cancelButton
                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignLeft
                    borderWidth: active ? 0 : 1
                    backgroundColor: active ? Style.colorBasePurple : "transparent"
                    opacityOnPressed: 0.7
                    text: qsTr("Cancel")
                    textColor: active ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                    fontWeight: Font.Bold
                    fontSize: Fonts.size12

                    onClicked: root.close()
                    Keys.onReturnPressed: root.close()
                    Keys.onLeftPressed: internal.focusApplyButton()
                    Keys.onRightPressed: internal.focusDeleteButton()
                    Keys.onTabPressed: internal.focusDeleteButton()
                }

                Item {
                    id: widthFiller
                    Layout.fillWidth: true
                }

                MButton {
                    id: deleteButton
                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignRight
                    borderWidth: active ? 0 : 1
                    backgroundColor: active ? Style.colorRed : "transparent"
                    opacityOnPressed: 0.7
                    text: qsTr("Delete")
                    textColor: active ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                    fontWeight: Font.Bold
                    fontSize: Fonts.size12
                    imagePath: active ? Icons.trashHighlighted : Icons.trash
                    imageSize: 17
                    imageSpacing: 10

                    onClicked: acceptDeletionPopup.open()
                    Keys.onReturnPressed: acceptDeletionPopup.open()
                    Keys.onLeftPressed: internal.focusCancelButton()
                    Keys.onTabPressed: internal.focusApplyButton()
                }
            }
        }
    }

    FileDialog {
        id: chooseImageDialog
        acceptLabel: qsTr("Select")
        fileMode: FileDialog.OpenFile
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        nameFilters: [qsTr(
                "All files") + "(*.png *.jpg *.jpg *.jpeg)", "png " + qsTr(
                "files") + " (*.png)", "jpg " + qsTr(
                "files") + " (*.jpg)", "jpeg " + qsTr("files") + " (*.jpeg)"]

        onAccepted: bookCover.source = file
    }

    QtObject {
        id: internal
        property string placeholderText: qsTr("Unknown")

        function focusApplyButton() {
            cancelButton.active = false
            deleteButton.active = false

            applyButton.active = true
            applyButton.forceActiveFocus()
        }

        function focusCancelButton() {
            applyButton.active = false
            deleteButton.active = false

            cancelButton.active = true
            cancelButton.forceActiveFocus()
        }

        function focusDeleteButton() {
            applyButton.active = false
            cancelButton.active = false

            deleteButton.active = true
            deleteButton.forceActiveFocus()
        }

        function saveData() {
            var operationsMap = {}

            if (titleField.text !== Globals.selectedBook.title)
                operationsMap[LibraryController.MetaProperty.Title] = titleField.text

            if (authorsField.text !== Globals.selectedBook.authors)
                operationsMap[LibraryController.MetaProperty.Authors] = authorsField.text

            if (languageComboBox.text !== Globals.selectedBook.language
                    && languageComboBox.text != "")
                operationsMap[LibraryController.MetaProperty.Language] = languageComboBox.text

            if (documentCreatorField.text !== Globals.selectedBook.creator
                    && documentCreatorField.text != "")
                operationsMap[LibraryController.MetaProperty.Creator] = documentCreatorField.text

            if (creationDateField.text !== Globals.selectedBook.creationDate
                    && creationDateField.text != internal.placeholderText)
                operationsMap[LibraryController.MetaProperty.CreationDate] = creationDateField.text

            if (formatField.text !== Globals.selectedBook.format
                    && formatField.text != internal.placeholderText)
                operationsMap[LibraryController.MetaProperty.Format] = formatField.text

            LibraryController.updateBook(Globals.selectedBook.uuid,
                                         operationsMap)

            // Handle book cover specially
            if (bookCover.source != Globals.selectedBook.coverPath)
                // Needs to be !=, the types are different (QUrl and QString)
                LibraryController.changeBookCover(Globals.selectedBook.uuid,
                                                  bookCover.source)
        }

        function setupPopup() {
            applyButton.forceActiveFocus()
            applyButton.active = true
            cancelButton.active = false
            deleteButton.active = false

            dataSideScrollView.contentItem.contentY = 0
        }

        function loadData() {
            if (Globals.selectedBook.coverPath !== "")
                bookCover.source = Qt.binding(function () {
                    return Globals.selectedBook.coverPath
                })

            if (Globals.selectedBook.language !== "")
                languageComboBox.setDefaultItem(Globals.selectedBook.language)
        }

        function unloadData() {
            languageComboBox.deselectCurrenItem()
        }
    }
}
