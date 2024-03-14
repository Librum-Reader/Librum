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
        LibraryController.bookTitleModel.extension = "pdf"
    }

    onClosed: {
        internal.clearSelectedItems()
        internal.refreshModel()

        LibraryController.bookTitleModel.showOnlyDownloaded = false
        LibraryController.bookTitleModel.extension = ""
    }

    Connections {
        target: ToolsController

        function onMergingFinished(success) {
            if (success) {
                showAlert("success", qsTr("Merge succeeded"),
                          qsTr("The merged book was added to your library."))
            } else {
                showAlert("error", qsTr("Merge failed"),
                          qsTr("The merged failed. Please try again."))
            }

            mergeButton.loading = false

            internal.clearSelectedItems()
            internal.refreshModel()
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
                text: qsTr("Merge")
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

            onCountChanged: internal.refreshModel()
            onEscPressed: root.close()
        }

        ListView {
            id: listView
            property int scrollbarExtraWidth: 16

            Layout.fillHeight: true
            Layout.maximumHeight: contentHeight
            Layout.topMargin: 24
            Layout.preferredWidth: bookSelector.width + scrollbarExtraWidth
            Layout.leftMargin: scrollbarExtraWidth
            Layout.alignment: Qt.AlignHCenter
            model: 0
            spacing: 2
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            boundsMovement: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar {
                anchors.top: parent.top
                anchors.left: parent.right
                anchors.bottom: parent.bottom
                anchors.leftMargin: -width
            }

            delegate: Rectangle {
                id: delRoot
                property string title: bookSelector.selectedItems[modelData]
                                       === undefined ? "" : bookSelector.selectedItems[modelData].title
                property string uuid: bookSelector.selectedItems[modelData]
                                      === undefined ? "" : bookSelector.selectedItems[modelData].uuid

                height: 52
                radius: 4
                color: Style.colorToolListItemBackground
                border.width: 1
                border.color: Style.colorToolListItemBorder
                width: listView.width - listView.scrollbarExtraWidth

                RowLayout {
                    anchors.fill: parent

                    Label {
                        Layout.fillWidth: true
                        Layout.leftMargin: 14
                        Layout.alignment: Qt.AlignVCenter
                        text: delRoot.title
                        color: Style.colorText
                        font.pointSize: Fonts.size12
                        elide: Text.ElideRight
                    }

                    MButton {
                        Layout.preferredHeight: 38
                        Layout.preferredWidth: 34
                        Layout.rightMargin: 2
                        backgroundColor: Style.colorContainerBackground
                        opacityOnPressed: 0.7
                        borderColor: "transparent"
                        radius: 6
                        borderColorOnPressed: Style.colorButtonBorder
                        imagePath: Icons.arrowheadNextIcon
                        imageRotation: -90
                        imageSize: 22

                        onClicked: internal.moveBookUp(delRoot.uuid)
                    }

                    MButton {
                        Layout.preferredHeight: 38
                        Layout.preferredWidth: 34
                        Layout.rightMargin: 2
                        backgroundColor: Style.colorContainerBackground
                        opacityOnPressed: 0.7
                        borderColor: "transparent"
                        radius: 6
                        borderColorOnPressed: Style.colorButtonBorder
                        imagePath: Icons.arrowheadNextIcon
                        imageRotation: 90
                        imageSize: 22

                        onClicked: internal.moveBookDown(delRoot.uuid)
                    }

                    MButton {
                        Layout.preferredHeight: 38
                        Layout.preferredWidth: 34
                        Layout.rightMargin: 12
                        backgroundColor: Style.colorRed
                        opacityOnPressed: 0.7
                        borderColor: "transparent"
                        radius: 6
                        borderColorOnPressed: Style.colorButtonBorder
                        imagePath: Icons.xIcon
                        imageSize: 16

                        onClicked: internal.removeBookFromSelection(
                                       delRoot.uuid)
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.maximumHeight: 24
            Layout.minimumHeight: 4
        }

        MButton {
            id: mergeButton
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
            text: qsTr("Merge")

            onClicked: {
                if (bookSelector.selectedItems.length <= 1) {
                    return
                }

                mergeButton.loading = true

                var name = "Merged: " + bookSelector.selectedItems[0].title
                ToolsController.merge(name, bookSelector.selectedItems.map(
                                          x => x.filePath))
            }
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 6
            visible: mergeButton.opacity !== 1
            text: qsTr("Select two or more books to merge.")
            color: Style.colorText
            font.pointSize: Fonts.size10dot25
            elide: Text.ElideRight
        }

        Item {
            Layout.fillHeight: true
        }
    }

    QtObject {
        id: internal

        function refreshModel() {
            listView.model = 0
            listView.model = bookSelector.selectedItems.length

            mergeButton.opacity = bookSelector.selectedItems.length >= 2 ? 1 : 0.6
        }

        function clearSelectedItems() {
            for (var i = 0; i < bookSelector.list.count; i++) {
                if (bookSelector.list.itemAtIndex(i) === null)
                    continue

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
