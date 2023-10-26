import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import QtQml.Models
import Librum.style
import Librum.icons
import Librum.controllers

Popup {
    id: root
    property string currentlySelectedData
    signal itemSelected

    implicitWidth: 200
    padding: 8
    focus: true
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
    background: Rectangle {
        color: Style.colorPopupBackground
        border.width: 1
        border.color: Style.colorContainerBorder
        radius: 4
        antialiasing: true
    }

    onOpened: listView.currentIndex = -1
    onAboutToHide: {
        internal.stopRenamingCurrentTag()
        tagOptionsPopup.close()
    }

    ColumnLayout {
        id: mainLayout
        width: parent.width

        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            Layout.maximumHeight: 208
            maximumFlickVelocity: 550
            keyNavigationEnabled: true
            currentIndex: -1
            clip: true
            focus: true
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar {}
            model: UserController.tagsModel
            delegate: MBaseListItem {
                width: listView.width
                height: 32
                containingListview: listView
                fontSize: Fonts.bigSize
                fontColor: Style.colorLightText
                checkBoxStyle: false

                onClicked: (mouse, index) => internal.selectItem(index)
                onRenamed: internal.renameTag(index, text)
                onRightClicked: (mouse, index) => {
                                    internal.stopRenamingCurrentTag()

                                    // Calculate the popup position
                                    let absoluteMousePosition = mapToItem(
                                        mainLayout, mouse.x, mouse.y)
                                    tagOptionsPopup.x = absoluteMousePosition.x + 2
                                    tagOptionsPopup.y = absoluteMousePosition.y + 2

                                    // Open the popup
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

    MRightClickMenu {
        id: tagOptionsPopup
        property int index
        property string originalTextOfLastEdited

        visible: false

        objectModel: ObjectModel {
            MRightClickMenuItem {
                width: tagOptionsPopup.width
                imagePath: Icons.plusSquare
                imageSize: 17
                text: "Add"

                onClicked: {
                    internal.selectItem(tagOptionsPopup.index)
                    tagOptionsPopup.close()
                }
            }

            MRightClickMenuItem {
                width: tagOptionsPopup.width
                imagePath: Icons.edit
                imageSize: 17
                text: "Rename"

                onClicked: internal.startRenamingTag(tagOptionsPopup.index)
            }

            MRightClickMenuItem {
                width: tagOptionsPopup.width
                imagePath: Icons.trash
                imageSize: 16
                text: "Delete"

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
            let tagName = listView.itemAtIndex(index).getContent()
            let uuid = UserController.getTagUuidForName(tagName)

            let success = UserController.deleteTag(uuid)
            if (success) {
                LibraryController.removeAllTagsWithUuid(uuid)
            }

            tagOptionsPopup.close()
        }

        function selectItem(index) {
            // Stop the renaming of the currentItem
            let oldItem = listView.itemAtIndex(tagOptionsPopup.index)
            if (oldItem !== null && oldItem.renameable)
                oldItem.stopRenaming()

            let newSelected = listView.itemAtIndex(index)

            root.currentlySelectedData = newSelected.getContent()
            root.itemSelected()
            root.close()
        }

        function stopRenamingCurrentTag(saveText = true) {
            let currentItem = listView.itemAtIndex(tagOptionsPopup.index)
            if (currentItem !== null && currentItem.renameable)
                currentItem.stopRenaming(saveText)
        }
    }
}
