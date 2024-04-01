import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import QtQml.Models
import Librum.style
import Librum.icons
import Librum.controllers
import Librum.fonts

Popup {
    id: root
    focus: true
    padding: 0
    implicitWidth: 168
    closePolicy: Popup.CloseOnReleaseOutsideParent | Popup.CloseOnEscape
    background: Rectangle {
        color: "transparent"
    }

    onOpenedChanged: if (opened)
                         listView.forceActiveFocus()
    onAboutToHide: {
        // When clicking the tagOptionsPopup while its over the popup edges,
        // root automatically closes because of its closing policy. We don't want this.
        if (tagOptionsPopup.opened)
            root.open()

        internal.stopRenamingCurrentTag(false)
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 0

        Image {
            id: triangleDecoration
            Layout.leftMargin: 14
            Layout.bottomMargin: -1
            source: Icons.popupDroplet
        }

        Pane {
            id: container
            Layout.fillHeight: true
            Layout.fillWidth: true
            padding: 6
            background: Rectangle {
                color: Style.colorPopupBackground
                border.width: 1
                border.color: Style.colorContainerBorder
                radius: 6
                antialiasing: true
            }

            ColumnLayout {
                id: itemLayout
                width: parent.width

                ListView {
                    id: listView
                    Layout.fillWidth: true
                    Layout.preferredHeight: contentHeight
                    Layout.maximumHeight: 200
                    maximumFlickVelocity: 550
                    currentIndex: -1
                    keyNavigationEnabled: true
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds
                    ScrollBar.vertical: ScrollBar {}
                    model: UserController.tagsModel
                    delegate: MBaseListItem {
                        width: listView.width
                        containingListview: listView

                        onClicked: internal.selectTag(index)
                        onRenamed: (index, text) => internal.renameTag(index,
                                                                       text)
                        onRightClicked: (mouse, index) => {
                                            internal.stopRenamingCurrentTag()

                                            // Calculate tagOptionsPopup position
                                            let mousePosition = mapToItem(
                                                container, mouse.x, mouse.y)
                                            tagOptionsPopup.x = mousePosition.x + 1
                                            tagOptionsPopup.y = mousePosition.y + 6

                                            // Open tagOptionsPopup
                                            tagOptionsPopup.index = index
                                            tagOptionsPopup.open()
                                        }

                        // Function required by MBaseListItem
                        function getContent() {
                            return model.name
                        }
                    }

                    // Close popup when scrolling
                    onContentYChanged: tagOptionsPopup.close()
                }
            }
        }
    }

    MRightClickMenu {
        id: tagOptionsPopup
        property int index: -1
        property string originalTextOfLastEdited

        visible: false

        objectModel: ObjectModel {
            MRightClickMenuItem {
                width: tagOptionsPopup.width
                imagePath: Icons.checkCircle
                imageSize: 17
                text: qsTr("Select")

                onClicked: {
                    internal.selectTag(tagOptionsPopup.index)
                    tagOptionsPopup.close()
                }
            }

            MRightClickMenuItem {
                width: tagOptionsPopup.width
                imagePath: Icons.edit
                imageSize: 17
                text: qsTr("Rename")

                onClicked: internal.startRenamingTag(tagOptionsPopup.index)
            }

            MRightClickMenuItem {
                width: tagOptionsPopup.width
                imagePath: Icons.trash
                imageSize: 16
                text: qsTr("Delete")

                onClicked: internal.deleteTag(tagOptionsPopup.index)
            }
        }
    }

    QtObject {
        id: internal


        /*
          Innitiate the visual renaming proccess of tag at @index
          */
        function startRenamingTag(index) {
            let currentItem = listView.itemAtIndex(index)
            tagOptionsPopup.originalTextOfLastEdited = currentItem.getContent()

            currentItem.startRenaming()
            tagOptionsPopup.close()
        }

        function renameTag(index, text) {
            // Get tag to rename
            let currentItem = listView.itemAtIndex(index)
            let tagName = currentItem.getContent()
            let uuid = UserController.getTagUuidForName(tagName)

            // Rename tag
            let success = UserController.renameTag(uuid, text)
            if (success) {
                let oldText = tagOptionsPopup.originalTextOfLastEdited
                LibraryController.renameTags(oldText, text)
            }
        }

        function deleteTag(index) {
            // Get tag uuid
            let tagName = listView.itemAtIndex(index).getContent()
            let uuid = UserController.getTagUuidForName(tagName)

            // Delete tag
            let success = UserController.deleteTag(uuid)
            if (success) {
                LibraryController.removeAllTagsWithUuid(uuid)
            }

            tagOptionsPopup.close()
        }

        function selectTag(index) {
            internal.stopRenamingCurrentTag()

            // Set ListView properties
            listView.currentIndex = index
            listView.currentItem.selected = !listView.currentItem.selected

            if (listView.itemAtIndex(index).selected)
                LibraryController.libraryModel.addFilterTag(
                            listView.currentItem.getContent())
            else
                LibraryController.libraryModel.removeFilterTag(
                            listView.currentItem.getContent())
        }

        function stopRenamingCurrentTag(saveText = true) {
            let currentItem = listView.itemAtIndex(tagOptionsPopup.index)
            if (currentItem !== null && currentItem.renameable)
                currentItem.stopRenaming(saveText)
        }
    }

    function clearSelections() {
        for (var i = 0; i < listView.count; i++) {
            if (listView.itemAtIndex(i) !== null)
                listView.itemAtIndex(i).selected = false
        }
    }

    function hasAtLeastOneTagSelected() {
        for (var i = 0; i < listView.count; i++) {
            if (listView.itemAtIndex(i) !== null && listView.itemAtIndex(
                        i).selected === true)
                return true
        }

        return false
    }
}
