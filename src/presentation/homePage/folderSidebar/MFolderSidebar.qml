import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.controllers
import Librum.style
import Librum.icons
import Librum.fonts

Item {
    id: root
    property bool opened: false
    property int openedWidth: 280

    width: 0


    /*
      Adds a border to the whole settings sidebar
     */
    Rectangle {
        id: background
        anchors.fill: parent
        color: Style.colorSettingsSidebarBackground

        Rectangle {
            id: rightBorder
            width: 1
            height: parent.height
            anchors.right: parent.right
            color: Style.colorContainerBorder
        }
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 0
        visible: root.opened

        Label {
            Layout.topMargin: 28
            Layout.leftMargin: 25
            text: qsTr("Organize")
            font.pointSize: Fonts.size19
            font.bold: true
            color: Style.colorTitle
        }

        Rectangle {
            id: titleSeparator
            Layout.preferredWidth: 56
            Layout.preferredHeight: 2
            Layout.topMargin: 18
            Layout.leftMargin: 26
            color: Style.colorDarkSeparator
        }

        MFolderSidebarItem {
            Layout.fillWidth: true
            Layout.leftMargin: 10
            Layout.rightMargin: 12
            Layout.topMargin: 18
            title: qsTr("All Books")
            icon: Icons.bookClosed
        }

        MFolderSidebarItem {
            Layout.fillWidth: true
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            title: qsTr("Unsorted")
            icon: Icons.unsorted
        }

        Rectangle {
            id: sidebarItemSeparator
            Layout.fillWidth: true
            Layout.preferredHeight: 2
            Layout.topMargin: 18
            Layout.leftMargin: 26
            Layout.rightMargin: 26
            color: Style.colorDarkSeparator
        }

        RowLayout {
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 20
            Layout.topMargin: 18

            Label {
                id: foldersTitle
                Layout.leftMargin: 26
                Layout.alignment: Qt.AlignLeft
                text: qsTr("Folders")
                font.pointSize: Fonts.size10dot5
                font.weight: Font.Bold
                color: Style.colorTitle
            }

            Item {
                Layout.preferredHeight: 21
                Layout.preferredWidth: 21
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.rightMargin: 26
                opacity: addFolderButtonArea.pressed ? 0.6 : 1

                Rectangle {
                    anchors.fill: parent
                    radius: height
                    color: "white"
                    opacity: 0.08
                }

                Image {
                    anchors.centerIn: parent
                    source: Icons.addFolderPlus
                    sourceSize.width: 15
                    fillMode: Image.PreserveAspectFit
                }

                MouseArea {
                    id: addFolderButtonArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: addFolderPopup.open()
                }
            }
        }

        Pane {
            id: treeViewContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 4
            verticalPadding: 6
            horizontalPadding: 4
            background: Rectangle {
                color: "transparent"
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

                TreeView {
                    id: treeView
                    property int indent: 18

                    anchors.fill: parent
                    anchors.margins: 1
                    anchors.rightMargin: scrollBar.isEnabled ? 18 : 6
                    anchors.leftMargin: 6
                    clip: true
                    focus: true

                    model: FolderController.foldersModel
                    delegate: Pane {
                        id: treeNode
                        required property string name
                        required property string uuid
                        required property TreeView treeView
                        required property bool expanded
                        required property int hasChildren
                        required property int depth

                        implicitWidth: treeView.width
                        width: implicitWidth
                        implicitHeight: 30
                        padding: 0

                        background: Rectangle {
                            anchors.fill: parent
                            color: backgroundArea.containsMouse ? "white" : "transparent"
                            opacity: 0.08
                            radius: 4
                        }

                        RowLayout {
                            id: nodeLayout
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 0

                            Image {
                                id: indicator
                                Layout.preferredWidth: implicitWidth
                                Layout.leftMargin: treeNode.depth * treeView.indent + 2
                                Layout.alignment: Qt.AlignVCenter
                                visible: treeNode.hasChildren
                                opacity: nodeLabelTrigger.pressed
                                         || indicatorArea.pressed ? 0.7 : 1
                                source: Icons.arrowheadNextIcon
                                sourceSize.width: 22
                                fillMode: Image.PreserveAspectFit
                                rotation: treeNode.expanded ? 90 : 0

                                MouseArea {
                                    id: indicatorArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor

                                    onClicked: treeView.toggleExpanded(row)
                                }
                            }

                            Image {
                                id: icon
                                Layout.preferredWidth: implicitWidth
                                Layout.leftMargin: treeNode.hasChildren ? indicator.width * 0.1 : indicator.width * 1.1 + depth * treeView.indent + 2
                                Layout.alignment: Qt.AlignVCenter
                                opacity: nodeLabelTrigger.pressed
                                         || iconArea.pressed ? 0.7 : 1
                                source: Icons.folder
                                sourceSize.width: 17
                                fillMode: Image.PreserveAspectFit

                                MouseArea {
                                    id: iconArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor

                                    onClicked: {

                                    }
                                }
                            }

                            Text {
                                id: treeNodeLabel
                                Layout.fillWidth: true
                                Layout.leftMargin: 6
                                Layout.topMargin: 1
                                Layout.alignment: Qt.AlignVCenter
                                clip: true
                                color: Style.colorText
                                opacity: nodeLabelTrigger.pressed ? 0.7 : 1
                                font.pointSize: Fonts.size10dot25
                                font.weight: Font.Medium
                                elide: Text.ElideRight
                                text: treeNode.name

                                MouseArea {
                                    id: nodeLabelTrigger
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                    acceptedButtons: Qt.LeftButton | Qt.RightButton

                                    onClicked: mouse => {
                                                   if (mouse.button === Qt.RightButton) {
                                                       let point = mapToItem(
                                                           root, mouse.x,
                                                           mouse.y)

                                                       rightclickPopup.x = point.x
                                                       rightclickPopup.y = point.y + 5

                                                       if (point.y + 5 + rightclickPopup.height
                                                           > baseRoot.height) {
                                                           rightclickPopup.y = point.y
                                                           - rightclickPopup.height - 5
                                                       }

                                                       rightclickPopup.open()
                                                   }
                                               }
                                }
                            }

                            Image {
                                id: threeDotsIcon
                                Layout.preferredWidth: implicitWidth
                                Layout.rightMargin: 18
                                Layout.alignment: Qt.AlignVCenter
                                opacity: threeDotsIconArea.pressed ? 0.7 : 1
                                source: Icons.dots
                                sourceSize.width: 16
                                fillMode: Image.PreserveAspectFit
                                visible: backgroundArea.containsMouse

                                MouseArea {
                                    id: threeDotsIconArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor

                                    onClicked: {
                                        let point = mapToItem(
                                                root, threeDotsIconArea.x,
                                                threeDotsIconArea.y)

                                        rightclickPopup.x = point.x + threeDotsIconArea.width / 2
                                                - rightclickPopup.implicitWidth / 2
                                        rightclickPopup.y = point.y + threeDotsIconArea.height + 2

                                        // If the popup is outside the window, move it up
                                        if (point.y + 5 + rightclickPopup.height
                                                > baseRoot.height) {
                                            rightclickPopup.y = point.y - rightclickPopup.height - 2
                                        }

                                        rightclickPopup.open()
                                    }
                                }
                            }
                        }

                        MouseArea {
                            id: backgroundArea
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            acceptedButtons: Qt.NoButton
                        }
                    }
                }

                Component.onCompleted: {
                    // contentItem is the TreeView's underlying Flickable
                    contentItem.flickDeceleration = 10000
                    contentItem.maximumFlickVelocity = 2000
                    contentItem.boundsBehavior = Flickable.StopAtBounds
                    contentItem.boundsMovement = Flickable.StopAtBounds
                }
            }
        }
    }

    MAddFolderPopup {
        id: addFolderPopup
        x: Math.round(baseRoot.width / 2 - implicitWidth / 2 - sidebar.width)
        y: Math.round(baseRoot.height / 2 - implicitHeight / 2 - 30)
        visible: false
    }

    MFolderSidebarItemRigthclickPopup {
        id: rightclickPopup
        visible: false
    }

    function open() {
        opened = true
        width = openedWidth
    }

    function close() {
        opened = false
        width = 0
    }

    function toggle() {
        if (opened)
            close()
        else
            open()
    }
}
