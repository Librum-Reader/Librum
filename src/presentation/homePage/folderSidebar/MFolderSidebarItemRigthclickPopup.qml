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
    implicitWidth: 164
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

            Layout.fillWidth: true
            Layout.preferredHeight: 32
            horizontalPadding: 8
            verticalPadding: 6

            background: Rectangle {
                anchors.fill: parent
                color: menuItemArea.containsMouse ? "white" : "transparent"
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
            }
        }

        MenuItem {
            text: "Create subfolder"
        }

        MenuItem {
            text: "Edit"
        }

        MenuItem {
            text: "Move to"
            Layout.bottomMargin: 6
        }

        Rectangle {
            Layout.preferredWidth: parent.width * 0.94
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: 1
            color: Style.colorSeparator
        }

        MenuItem {
            Layout.topMargin: 6
            text: "Delete"
            textColor: Style.colorRed
        }
    }
}
