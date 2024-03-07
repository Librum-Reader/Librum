import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons
import Librum.fonts
import Librum.controllers

Popup {
    id: root
    property bool checkBoxStyle: true
    property var selectedItems: []
    signal itemsChanged

    implicitWidth: 200
    implicitHeight: 360
    background: Rectangle {
        color: Style.colorControlBackground
        border.color: Style.colorContainerBorder
        radius: 5
        border.width: 2
    }

    ColumnLayout {
        id: layout
        width: parent.width

        ListView {
            id: listView
            property MBaseListItem currentSelected

            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            Layout.maximumHeight: root.height - root.verticalPadding - 12
            maximumFlickVelocity: 550
            keyNavigationEnabled: true
            currentIndex: 0
            clip: true
            focus: true
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar {}

            model: LibraryController.bookTitleModel

            delegate: MBaseListItem {
                width: listView.width
                height: 36
                padding: 10
                containingListview: listView
                fontSize: Fonts.size12
                fontColor: Style.colorLightText

                checkBoxStyle: root.checkBoxStyle
                checkBoxImageSize: 10
                checkBoxSize: 20
                onClicked: (mouse, index) => {
                               listView.currentIndex = index

                               let uuid = listView.currentItem.getItemProperty(
                                   "uuid")
                               var indexInParent = root.selectedItems.findIndex(
                                   x => x.uuid === uuid)

                               if (indexInParent !== -1) {
                                   root.selectedItems.splice(indexInParent, 1)
                                   listView.currentItem.selected = false
                               } else {
                                   root.selectedItems.push({
                                                               "uuid": uuid,
                                                               "title": listView.currentItem.getItemProperty(
                                                                            "title"),
                                                               "filePath": listView.currentItem.getItemProperty("filePath")
                                                           })
                                   listView.currentItem.selected = true
                               }

                               root.itemsChanged()
                           }

                // Overwrite the way the model's text is queried
                function getContent() {
                    return model["title"]
                }
            }

            // Keys.onReturnPressed: if (listView.currentIndex !== -1)
            // root.selectItem(listView.currentIndex)
            // Component.onCompleted: if (root.defaultIndex != -1)
            // root.selectItem(listView.currentIndex,
            // true)
        }
    }

    function moveBookUp(uuid) {
        var indexInParent = root.selectedItems.findIndex(x => x.uuid === uuid)
        if (indexInParent === -1)
            return

        if (indexInParent === 0)
            return

        var temp = root.selectedItems[indexInParent]
        root.selectedItems.splice(indexInParent, 1)
        root.selectedItems.splice(indexInParent - 1, 0, temp)
        root.itemsChanged()
    }

    function moveBookDown(uuid) {
        var indexInParent = root.selectedItems.findIndex(x => x.uuid === uuid)
        if (indexInParent === -1)
            return

        if (indexInParent >= root.selectedItems.length - 1)
            return

        var temp = root.selectedItems[indexInParent]
        root.selectedItems.splice(indexInParent, 1)
        root.selectedItems.splice(indexInParent + 1, 0, temp)
        root.itemsChanged()
    }

    function removeBookFromSelection(uuid) {
        var indexInParent = root.selectedItems.findIndex(x => x.uuid === uuid)
        if (indexInParent === -1)
            return

        listView.currentIndex = indexInParent
        listView.currentItem.selected = false

        root.selectedItems.splice(indexInParent, 1)
        root.itemsChanged()
    }
}
