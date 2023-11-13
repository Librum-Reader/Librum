import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons
import Librum.controllers
import Librum.fonts

Item {
    id: root
    implicitWidth: 300
    implicitHeight: 600

    Rectangle {
        id: background
        anchors.fill: parent
        color: "transparent"

        ColumnLayout {
            id: layout
            anchors.fill: parent
            spacing: 0

            Label {
                id: header
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 26
                text: qsTr("Bookmarks")
                font.pointSize: Fonts.size14
                font.weight: Font.Medium
                color: Style.colorText
            }

            Rectangle {
                id: searchBox
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                Layout.margins: 12
                Layout.alignment: Qt.AlignBottom
                radius: 4
                color: Style.colorContainerBackground
                border.color: Style.colorButtonBorder

                TextField {
                    id: inputField
                    anchors.fill: parent
                    leftPadding: 15
                    rightPadding: 15
                    selectByMouse: true
                    color: Style.colorText
                    font.pointSize: Fonts.size11
                    font.weight: Font.Normal
                    placeholderText: qsTr("Search") + "..."
                    placeholderTextColor: Style.colorPlaceholderText
                    background: Rectangle {
                        anchors.fill: parent
                        radius: 4
                        color: "transparent"
                    }

                    onTextEdited: BookController.bookmarksModel.filterString = text
                }
            }

            Pane {
                id: container
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 12
                Layout.rightMargin: 12
                Layout.topMargin: 0
                Layout.bottomMargin: 12
                verticalPadding: 6
                horizontalPadding: 4
                background: Rectangle {
                    radius: 4
                    color: Style.colorContainerBackground
                    border.color: Style.colorContainerBorder
                }

                ScrollView {
                    id: scrollBar
                    property bool isEnabled: contentHeight > height
                    anchors.fill: parent

                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    onIsEnabledChanged: {
                        if (isEnabled)
                            ScrollBar.vertical.policy = ScrollBar.AlwaysOn
                        else
                            ScrollBar.vertical.policy = ScrollBar.AlwaysOff
                    }
                }

                ListView {
                    id: bookmarksView
                    property string lastAddedUuid
                    signal startRenaming(string uuid)

                    anchors.fill: parent
                    boundsBehavior: Flickable.StopAtBounds
                    model: BookController.bookmarksModel
                    clip: true

                    delegate: MBookmarkItem {
                        width: bookmarksView.width

                        onRightClicked: (mouse, index) => {
                                            bookmarksView.currentIndex = index

                                            // Calculate tagOptionsPopup position
                                            let mousePosition = mapToItem(
                                                container, mouse.x, mouse.y)
                                            bookmarkOptionsPopup.x = mousePosition.x + 14
                                            bookmarkOptionsPopup.y = mousePosition.y + 106

                                            // Open tagOptionsPopup
                                            bookmarkOptionsPopup.open()
                                        }
                    }
                }
            }

            MButton {
                id: addBookmarkButton
                Layout.fillWidth: true
                Layout.preferredHeight: 35
                Layout.leftMargin: 12
                Layout.rightMargin: 12
                Layout.bottomMargin: 12
                fontSize: Fonts.size10dot5
                backgroundColor: Style.colorContainerBackground
                borderColor: Style.colorButtonBorder
                opacityOnPressed: 0.7
                radius: 4
                text: qsTr("Add Bookmark")
                imageSize: 12

                onClicked: createNewBookmark()
            }
        }
    }

    MRightClickMenu {
        id: bookmarkOptionsPopup
        property string originalTextOfLastEdited

        objectModel: ObjectModel {
            MRightClickMenuItem {
                width: bookmarkOptionsPopup.width
                imagePath: Icons.checkCircle
                imageSize: 17
                //: Context: "Follow" the bookmark (go to its page)
                text: qsTr("Follow")

                onClicked: {
                    bookmarksView.currentItem.goToBookmark()
                    bookmarkOptionsPopup.close()
                }
            }

            MRightClickMenuItem {
                width: bookmarkOptionsPopup.width
                imagePath: Icons.edit
                imageSize: 17
                text: qsTr("Rename")

                onClicked: {
                    bookmarksView.currentItem.startRenaming()
                    bookmarkOptionsPopup.close()
                }
            }

            MRightClickMenuItem {
                width: bookmarkOptionsPopup.width
                imagePath: Icons.trash
                imageSize: 16
                text: qsTr("Delete")

                onClicked: {
                    BookController.removeBookmark(
                                bookmarksView.currentItem.uuid)
                    bookmarkOptionsPopup.close()
                }
            }
        }
    }

    function createNewBookmark() {
        let uuid = BookController.addBookmark(qsTr("New Bookmark"),
                                              BookController.currentPage,
                                              getYOffset())
        bookmarksView.lastAddedUuid = uuid
    }
}
