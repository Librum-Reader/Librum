import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.fonts
import Librum.icons

Pane {
    id: root
    required property int index
    required property string level
    required property string title
    required property string message
    signal destroyAlert(int index)

    horizontalPadding: 20
    verticalPadding: 16
    implicitWidth: 600
    implicitHeight: layout.implicitHeight + 2 * verticalPadding
    background: Rectangle {
        color: root.level === "success" ? Style.colorAlertSuccessBackground : (root.level === "error" ? Style.colorAlertErrorBackground : Style.colorAlertInfoBackground)
        radius: 4
        border.width: 1
        border.color: root.level === "success" ? Style.colorAlertSuccessBorder : (root.level === "error" ? Style.colorAlertErrorBorder : Style.colorAlertInfoBorder)
    }

    Component.onCompleted: selfDestructionTimer.start()

    ColumnLayout {
        id: layout
        width: parent.width
        spacing: 0

        RowLayout {
            spacing: 0
            Layout.fillWidth: true

            Image {
                id: levelIcon
                source: root.level === "success" ? Icons.checkCircleGreen : (root.level === "error" ? Icons.errorCircle : Icons.infoCircle)
                Layout.alignment: Qt.AlignVCenter
                sourceSize.width: 22
                fillMode: Image.PreserveAspectFit
            }

            Label {
                id: title
                text: root.title + (topMessage.visible ? ": " : "")
                Layout.leftMargin: 12
                Layout.alignment: Qt.AlignVCenter
                font.weight: Font.Bold
                font.pointSize: Fonts.size12
                color: Style.colorText
            }

            Label {
                id: topMessage
                Layout.fillWidth: true
                Layout.rightMargin: 8
                visible: contentWidth <= width
                text: root.message
                Layout.alignment: Qt.AlignVCenter
                font.pointSize: Fonts.size12
                color: Style.colorText
            }

            Item {
                Layout.fillWidth: true
            }

            Image {
                id: closeIcon
                source: Icons.xIcon
                Layout.alignment: Qt.AlignVCenter
                sourceSize.width: 22
                opacity: closeArea.pressed ? 0.6 : 1
                fillMode: Image.PreserveAspectFit

                MouseArea {
                    id: closeArea
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor

                    onClicked: root.destroyAlert(root.index)
                }
            }
        }

        Label {
            id: bottomMessage
            Layout.leftMargin: title.x
            Layout.fillWidth: true
            Layout.rightMargin: 8
            Layout.topMargin: 7
            visible: !topMessage.visible
            wrapMode: Text.WordWrap
            text: root.message
            Layout.alignment: Qt.AlignVCenter
            font.pointSize: Fonts.size12
            color: Style.colorText
        }
    }

    Timer {
        id: selfDestructionTimer
        interval: 2500

        onTriggered: root.destroyAlert(root.index)
    }
}
