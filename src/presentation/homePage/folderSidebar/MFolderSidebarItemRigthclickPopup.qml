import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import QtQml.Models
import Librum.style
import Librum.icons
import Librum.controllers
import Librum.fonts

Popup {
    id: root
    property string uuid

    width: Math.max(internal.minWidth, internal.dynamicWidth)
    height: mainLayout.height + 2 * padding
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

    ColumnLayout {
        id: mainLayout
        width: parent.width
        spacing: 0

        component MenuItem: Pane {
            id: menuItemRoot
            property string text
            property string textColor: Style.colorText
            property int implWidth: menuItemLabel.implicitWidth + 2 * horizontalPadding
            signal clicked

            LayoutMirroring.enabled: baseRoot.rightAlign
            LayoutMirroring.childrenInherit: true

            Layout.fillWidth: true
            Layout.preferredHeight: 32
            horizontalPadding: 8
            verticalPadding: 6

            background: Rectangle {
                anchors.fill: parent
                color: menuItemArea.containsMouse ? Style.colorTransparentHighlight : "transparent"
                opacity: menuItemArea.containsMouse ? 0.08 : 1
                radius: 4
            }

            Label {
                id: menuItemLabel
                text: menuItemRoot.text
                color: menuItemRoot.textColor
                font.pointSize: Fonts.size10dot5
                anchors.verticalCenter: parent.verticalCenter
                font.weight: Font.Medium
            }

            MouseArea {
                id: menuItemArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor

                onClicked: menuItemRoot.clicked()
            }
        }

        MenuItem {
            id: first
            text: qsTr("Create subfolder")

            onClicked: {
                addFolderPopup.parentUuid = root.uuid
                addFolderPopup.open()

                root.close()
            }
        }

        MenuItem {
            id: second
            text: qsTr("Edit")

            onClicked: {
                addFolderPopup.updateMode = true
                addFolderPopup.uuid = root.uuid
                addFolderPopup.open()

                root.close()
            }
        }

        MenuItem {
            id: third
            text: qsTr("Move to")
            Layout.bottomMargin: 6

            onClicked: {
                moveFolderToFolderPopup.folderUuid = root.uuid
                moveFolderToFolderPopup.headerItemText = "Root"
                moveFolderToFolderPopup.open()

                root.close()
            }
        }

        Rectangle {
            Layout.preferredWidth: parent.width * 0.94
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: 1
            color: Style.colorSeparator
        }

        MenuItem {
            id: fourth
            Layout.topMargin: 6
            text: qsTr("Delete")
            textColor: Style.colorRed

            onClicked: {
                deleteFolderPopup.uuid = root.uuid
                deleteFolderPopup.open()

                root.close()
            }
        }
    }

    QtObject {
        id: internal
        property int minWidth: 164
        property int dynamicWidth: Math.max(first.implWidth, second.implWidth,
                                            third.implWidth,
                                            fourth.implWidth) + 2 * root.padding
    }
}
