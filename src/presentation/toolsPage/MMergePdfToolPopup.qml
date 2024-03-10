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
        LibraryController.bookTitleModel.format = "pdf"
    }

    onClosed: {
        internal.clearSelectedItems()
        internal.refreshModel()

        LibraryController.bookTitleModel.showOnlyDownloaded = false
        LibraryController.bookTitleModel.format = ""
    }

    MFlickWrapper {
        id: flickWrapper
        anchors.fill: parent
        contentHeight: layout.height

        ColumnLayout {
            width: parent.width
            spacing: 0

            RowLayout {
                id: layout
                width: parent.width
                spacing: 0

                Label {
                    id: popupTitle
                    Layout.leftMargin: 36
                    text: qsTr("Merge PDFs")
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
                Layout.topMargin: 80

                onCountChanged: internal.refreshModel()
            }

            ListView {
                id: listView
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                Layout.maximumHeight: 400
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 16
                Layout.bottomMargin: 20
                model: 0
                delegate: Rectangle {
                    id: delRoot
                    property string title: bookSelector.selectedItems[modelData].title
                    property string uuid: bookSelector.selectedItems[modelData].uuid

                    radius: 4
                    color: "#222222"
                    border.width: 1
                    border.color: "#1E1E1E"
                    height: 36
                    width: parent.width

                    RowLayout {
                        anchors.fill: parent

                        Label {
                            Layout.topMargin: 4
                            Layout.fillWidth: true
                            Layout.leftMargin: 8
                            Layout.alignment: Qt.AlignVCenter
                            text: delRoot.title
                            color: Style.colorText
                            font.pointSize: Fonts.size14
                        }

                        MButton {
                            Layout.preferredHeight: 34
                            Layout.preferredWidth: 28
                            Layout.rightMargin: 8
                            backgroundColor: Style.colorContainerBackground
                            opacityOnPressed: 0.7
                            borderColor: "transparent"
                            radius: 6
                            borderColorOnPressed: Style.colorButtonBorder
                            imagePath: Icons.arrowheadNextIcon
                            imageRotation: -90
                            imageSize: 18

                            onClicked: internal.moveBookUp(delRoot.uuid)
                        }

                        MButton {
                            Layout.preferredHeight: 34
                            Layout.preferredWidth: 28
                            Layout.rightMargin: 4
                            backgroundColor: Style.colorContainerBackground
                            opacityOnPressed: 0.7
                            borderColor: "transparent"
                            radius: 6
                            borderColorOnPressed: Style.colorButtonBorder
                            imagePath: Icons.arrowheadNextIcon
                            imageRotation: 90
                            imageSize: 18

                            onClicked: internal.moveBookDown(delRoot.uuid)
                        }

                        MButton {
                            Layout.preferredHeight: 34
                            Layout.preferredWidth: 28
                            Layout.rightMargin: 4
                            backgroundColor: Style.colorRed
                            opacityOnPressed: 0.7
                            borderColor: "transparent"
                            radius: 6
                            borderColorOnPressed: Style.colorButtonBorder
                            imagePath: Icons.closePopupWhite
                            imageSize: 14

                            onClicked: internal.removeBookFromSelection(
                                           delRoot.uuid)
                        }
                    }
                }
            }

            MButton {
                id: mergeButton
                Layout.preferredWidth: 240
                Layout.preferredHeight: 40
                Layout.topMargin: 42
                borderWidth: 0
                backgroundColor: Style.colorBasePurple
                fontSize: Fonts.size12
                opacityOnPressed: 0.85
                textColor: Style.colorFocusedButtonText
                fontWeight: Font.Bold
                text: qsTr("Merge")

                onClicked: {
                    if (bookSelector.selectedItems.length <= 1) {
                        print("Select at least 2 books to merge.")
                        return
                    }

                    ToolsController.mergePdfs(bookSelector.selectedItems.map(
                                                  x => x.filePath))
                }
            }
        }
    }

    QtObject {
        id: internal

        function refreshModel() {
            listView.model = 0
            listView.model = bookSelector.selectedItems.length
        }

        function clearSelectedItems() {
            for (var i = 0; i < bookSelector.list.count; i++) {
                bookSelector.list.itemAtIndex(i).selected = false
            }

            bookSelector.selectedItems = []
        }

        function moveBookUp(uuid) {
            var indexInParent = bookSelector.selectedItems.findIndex(
                        x => x.uuid === uuid)
            if (indexInParent === -1)
                return

            if (indexInParent === 0)
                return

            var temp = bookSelector.selectedItems[indexInParent]
            bookSelector.selectedItems.splice(indexInParent, 1)
            bookSelector.selectedItems.splice(indexInParent - 1, 0, temp)
            internal.refreshModel()
        }

        function moveBookDown(uuid) {
            var indexInParent = bookSelector.selectedItems.findIndex(
                        x => x.uuid === uuid)
            if (indexInParent === -1)
                return

            if (indexInParent >= bookSelector.selectedItems.length - 1)
                return

            var temp = bookSelector.selectedItems[indexInParent]
            bookSelector.selectedItems.splice(indexInParent, 1)
            bookSelector.selectedItems.splice(indexInParent + 1, 0, temp)
            internal.refreshModel()
        }

        function removeBookFromSelection(uuid) {
            // Remove from selectedItems
            var indexInParent = bookSelector.selectedItems.findIndex(
                        x => x.uuid === uuid)
            if (indexInParent === -1)
                return
            bookSelector.selectedItems.splice(indexInParent, 1)

            // Deselect it in the bookSelector's dropdown list
            for (var i = 0; i < bookSelector.list.count; i++) {
                var item = bookSelector.list.itemAtIndex(i)
                if (item.getItemProperty("uuid") === uuid) {
                    item.selected = false
                    break
                }
            }

            internal.refreshModel()
        }
    }
}
