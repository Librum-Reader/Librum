import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.controllers
import Librum.fonts

Popup {
    id: root
    padding: 32
    bottomPadding: 42
    topPadding: 42
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    background: Rectangle {
        color: Style.colorPopupBackground
        radius: 6
        border.width: 1
        border.color: Style.colorContainerBorder
    }

    onOpened: {
        LibraryController.bookTitleModel.showOnlyDownloaded = true
        LibraryController.bookTitleModel.extension = "png,jpeg,jpg,svg"
    }
    onClosed: {
        LibraryController.bookTitleModel.showOnlyDownloaded = false
        LibraryController.bookTitleModel.extension = ""

        internal.resetState()
    }

    Connections {
        target: ToolsController

        function onConvertingFinished(success) {
            if (success) {
                showAlert("success", qsTr("Convertion succeeded"),
                          qsTr("The new book was added to your library."))
            } else {
                showAlert("error", qsTr("Convertion failed"),
                          qsTr("The conversion failed. Please try again."))
            }

            internal.resetState()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout {
            id: layout
            width: parent.width
            spacing: 0

            Label {
                id: popupTitle
                Layout.leftMargin: 36
                text: qsTr("Convert")
                font.weight: Font.Bold
                font.pointSize: Fonts.size20
                color: Style.colorTitle
            }

            Item {
                Layout.fillWidth: true
            }

            MButton {
                id: closeButton
                Layout.preferredHeight: 36
                Layout.preferredWidth: 36
                Layout.rightMargin: 18
                Layout.alignment: Qt.AlignRight
                backgroundColor: "transparent"
                opacityOnPressed: 0.7
                borderColor: "transparent"
                radius: 6
                borderColorOnPressed: Style.colorButtonBorder
                imagePath: Icons.closePopup
                imageSize: 16

                onClicked: root.close()
            }
        }

        MBookSelector {
            id: bookSelector
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 72
            Layout.preferredWidth: parent.width * 0.6
            multiSelect: false

            onCountChanged: {
                if (bookSelector.selectedItems.length > 0) {
                    bookSelector.input = ""
                    bookSelector.searchText = bookSelector.selectedItems[0].title

                    bookSelector.closePopup()
                    extractButton.opacity = 1
                    nameInput.placeholderContent = "Converted: "
                            + bookSelector.selectedItems[0].title
                    nameInput.giveFocus()
                } else {
                    bookSelector.searchText = internal.originalSearchText
                    extractButton.opacity = 0.6

                    nameInput.placeholderContent = "New Book"
                }
            }

            onEscPressed: root.close()
        }

        MLabeledInputBox {
            id: nameInput
            Layout.preferredWidth: parent.width * 0.6
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 48
            boxHeight: 46
            headerFontSize: Fonts.size11
            placeholderContent: "New Book"
            placeholderColor: Style.colorPlaceholderText
            headerText: "Output Name"
        }

        Item {
            Layout.fillHeight: true
            Layout.maximumHeight: 42
            Layout.minimumHeight: 8
        }

        MButton {
            id: extractButton
            Layout.preferredWidth: parent.width * 0.4
            Layout.preferredHeight: 44
            Layout.alignment: Qt.AlignHCenter
            opacity: 0.6
            borderWidth: 0
            backgroundColor: Style.colorBasePurple
            fontSize: Fonts.size12
            opacityOnPressed: 0.85
            textColor: Style.colorFocusedButtonText
            fontWeight: Font.Bold
            text: qsTr("Convert")

            onClicked: {
                if (bookSelector.selectedItems.length === 0) {
                    return
                }

                extractButton.loading = true

                var name = nameInput.text === "" ? nameInput.placeholderContent : nameInput.text
                ToolsController.imageToPdf(
                            name, bookSelector.selectedItems[0].filePath)
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }

    QtObject {
        id: internal
        property string originalSearchText

        Component.onCompleted: originalSearchText = bookSelector.searchText

        function resetState() {
            extractButton.loading = false
            internal.clearSelectedItems()
            bookSelector.searchText = internal.originalSearchText
            nameInput.clearText()
            extractButton.opacity = 0.6
        }

        function clearSelectedItems() {
            for (var i = 0; i < bookSelector.list.count; i++) {
                if (bookSelector.list.itemAtIndex(i) === null)
                    continue

                bookSelector.list.itemAtIndex(i).selected = false
            }

            bookSelector.selectedItems = []
        }
    }
}
