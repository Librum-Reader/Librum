import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.controllers

Item {
    id: root
    implicitWidth: 250
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
                text: "Bookmarks"
                font.pointSize: 14
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
                    font.pointSize: 11
                    font.weight: Font.Normal
                    placeholderText: "Search..."
                    placeholderTextColor: Style.colorPlaceholderText
                    background: Rectangle {
                        anchors.fill: parent
                        radius: 4
                        color: "transparent"
                    }

                    onTextEdited: BookController.tableOfContents.filterString = text
                }
            }

            Pane {
                id: treeViewContainer
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 12
                Layout.topMargin: 0
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
                    signal startRenaming(string uuid)

                    anchors.fill: parent
                    boundsBehavior: Flickable.StopAtBounds
                    model: BookController.bookmarksModel
                    clip: true

                    delegate: MBookmarkItem {
                        width: bookmarksView.width
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 32
                color: "green"
                opacity: 0.5

                Label {
                    anchors.centerIn: parent
                    text: "Add bookmark"
                    color: Style.colorText
                    font.pointSize: 11
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        print(getYOffset())
                        let uuid = BookController.addBookmark(
                                "New Bookmark", BookController.currentPage,
                                getYOffset())
                    }
                }
            }
        }
    }
}
