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

    // This way the right, draggable border is correctly placed in the middle of the border
    width: rightBorder.x + (opened ? rightBorder.width / 2 : 0)

    onOpenedChanged: {
        if (opened) {
            openAnim.start()
            return
        }

        internal.prevSidebarWidth = width
        closeAnim.start()
    }

    LayoutMirroring.enabled: baseRoot.rightAlign
    LayoutMirroring.childrenInherit: true

    Rectangle {
        id: background
        anchors.fill: parent
        visible: root.opened
        color: Style.colorSettingsSidebarBackground

        Item {
            id: rightBorder
            width: 13
            height: parent.height

            Item {
                id: leftBorderArea
                width: 6
                height: parent.height
                anchors.left: parent.left
            }

            Rectangle {
                id: visibleBorder
                anchors.left: leftBorderArea.right
                anchors.right: rightBorderArea.left
                height: parent.height
                color: Style.colorContainerBorder
            }

            Item {
                id: rightBorderArea
                width: 6
                height: parent.height
                anchors.right: parent.right
            }

            // The border should be pullable to resize the sidebar
            MouseArea {
                id: borderResizeArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.SizeHorCursor
                drag.target: rightBorder
                drag.axis: Drag.XAxis
                drag.minimumX: 180
                drag.maximumX: 440
            }
        }
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        visible: false
        opacity: 0
        spacing: 0

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
            selected: LibraryController.libraryModel.folder === "all"

            onClicked: LibraryController.libraryModel.folder = "all"
        }

        MFolderSidebarItem {
            Layout.fillWidth: true
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            title: qsTr("Unsorted")
            icon: Icons.unsorted
            selected: LibraryController.libraryModel.folder === "unsorted"

            onClicked: LibraryController.libraryModel.folder = "unsorted"
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
                Layout.rightMargin: 24
                opacity: addFolderButtonArea.pressed ? 0.5 : 1

                Rectangle {
                    anchors.fill: parent
                    radius: height
                    color: Style.colorTransparentHighlight
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
                        required property string uuid
                        required property string name
                        required property string color
                        required property string icon
                        required property string description
                        required property TreeView treeView
                        required property bool expanded
                        required property int hasChildren
                        required property int depth

                        Connections {
                            target: FolderController

                            function onExpandFolder(uuid) {
                                if (uuid === treeNode.uuid)
                                    treeView.expand(row)
                            }
                        }

                        implicitWidth: treeView.width
                        width: implicitWidth
                        implicitHeight: 32
                        padding: 0

                        background: Rectangle {
                            anchors.fill: parent
                            color: backgroundArea.containsMouse
                                   || LibraryController.libraryModel.folder
                                   === treeNode.uuid ? Style.colorTransparentHighlight : "transparent"
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
                                source: "qrc:/resources/images/folder_icons/"
                                        + treeNode.icon + ".svg"
                                sourceSize.width: 20
                                fillMode: Image.PreserveAspectFit


                                MouseArea {
                                    id: iconArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                    acceptedButtons: Qt.LeftButton | Qt.RightButton

                                    onClicked: mouse => nodeLayout.clickAction(
                                                   mouse)
                                }
                            }

                            Label {
                                id: treeNodeLabel
                                Layout.fillWidth: true
                                // To have a bigger clickable area
                                Layout.preferredHeight: treeNode.height
                                Layout.leftMargin: 6
                                Layout.rightMargin: threeDotsIcon.visible ? 8 : 18
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                                topPadding: 1
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

                                    onClicked: mouse => nodeLayout.clickAction(
                                                   mouse)
                                }
                            }

                            function clickAction(mouse) {
                                if (mouse.button === Qt.RightButton) {
                                    let point = mapToItem(root,
                                                          mouse.x, mouse.y)

                                    rightclickPopup.x = point.x
                                    rightclickPopup.y = point.y + 5

                                    if (point.y + 5 + rightclickPopup.height > baseRoot.height) {
                                        rightclickPopup.y = point.y - rightclickPopup.height - 5
                                    }

                                    rightclickPopup.uuid = treeNode.uuid
                                    rightclickPopup.open()
                                } else if (mouse.button === Qt.LeftButton) {
                                    LibraryController.libraryModel.folder = treeNode.uuid
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

                                        rightclickPopup.uuid = treeNode.uuid
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
                    contentItem.flickableDirection = Flickable.VerticalFlick
                }
            }
        }
    }

    ParallelAnimation {
        id: openAnim

        NumberAnimation {
            target: rightBorder
            to: internal.prevSidebarWidth
            property: "x"
            duration: 260
            easing.type: Easing.InOutQuad
        }

        NumberAnimation {
            target: layout
            from: 0
            to: 1
            property: "opacity"
            duration: 320
            easing.type: Easing.InOutQuad
        }

        PropertyAnimation {
            target: layout
            to: true
            property: "visible"
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }

    ParallelAnimation {
        id: closeAnim

        NumberAnimation {
            target: rightBorder
            to: 0
            property: "x"
            duration: 240
            easing.type: Easing.InOutQuad
        }

        NumberAnimation {
            target: layout
            from: 1
            to: 0
            property: "opacity"
            duration: 180
            easing.type: Easing.InOutQuad
        }

        PropertyAnimation {
            target: layout
            to: false
            property: "visible"
            duration: 300
            easing.type: Easing.InOutQuad
        }
    }

    MAddFolderPopup {
        id: addFolderPopup
        x: Math.round(baseRoot.width / 2 - implicitWidth / 2 - sidebar.width)
        y: Math.round(baseRoot.height / 2 - implicitHeight / 2 - 30)
    }

    MDeleteFolderPopup {
        id: deleteFolderPopup
        x: Math.round(baseRoot.width / 2 - implicitWidth / 2 - sidebar.width)
        y: Math.round(baseRoot.height / 2 - implicitHeight / 2 - 30)
    }

    MMoveToFolderPopup {
        id: moveFolderToFolderPopup
        x: Math.round(baseRoot.width / 2 - implicitWidth / 2 - sidebar.width)
        y: Math.round(baseRoot.height / 2 - implicitHeight / 2 - 30)
    }

    MFolderSidebarItemRigthclickPopup {
        id: rightclickPopup
        visible: false
    }

    MSelectColorPopup {
        id: selectColorPopup
        x: Math.round(baseRoot.width / 2 - implicitWidth / 2 - sidebar.width)
        y: Math.round(baseRoot.height / 2 - implicitHeight / 2 - 30)
    }

    MDescriptionPopup {
        id: descriptionPopup
        x: Math.round(baseRoot.width / 2 - implicitWidth / 2 - sidebar.width)
        y: Math.round(baseRoot.height / 2 - implicitHeight / 2 - 30)
    }

    MSelectIconPopup {
        id: selectIconPopup
        x: Math.round(baseRoot.width / 2 - implicitWidth / 2 - sidebar.width)
        y: Math.round(baseRoot.height / 2 - implicitHeight / 2 - 30)
    }

    function open() {
        opened = true
    }

    function close() {
        opened = false
    }

    function toggle() {
        if (opened)
            close()
        else
            open()
    }

    QtObject {
        id: internal
        property int prevSidebarWidth: 280
    }
}
