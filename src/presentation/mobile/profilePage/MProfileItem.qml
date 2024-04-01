import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.fonts
import Librum.icons

Pane {
    id: root
    property alias text: label.text
    property alias icon: icon.source
    property int iconWidth: 22
    signal clicked

    implicitHeight: 44
    horizontalPadding: 10
    verticalPadding: 0
    background: Rectangle {
        id: clickHighlight
        anchors.fill: parent
        color: Style.colorTransparentHighlight
        opacity: tapHandler.pressed ? 0.03 : 0
        radius: 10
    }

    RowLayout {
        spacing: 0
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter

        IconImage {
            id: icon
            Layout.alignment: Qt.AlignVCenter
            opacity: tapHandler.pressed ? 0.75 : 1
            sourceSize.width: root.iconWidth
            fillMode: Image.PreserveAspectFit
            color: Style.colorBasePurple
        }

        Label {
            id: label
            Layout.alignment: Qt.AlignVCenter
            Layout.leftMargin: 14
            font.pointSize: 16
            font.weight: Font.Medium
            color: Style.colorText
            opacity: tapHandler.pressed ? 0.75 : 1
        }

        Item {
            Layout.fillWidth: true
        }

        IconImage {
            id: arrow
            Layout.alignment: Qt.AlignVCenter
            Layout.rightMargin: -8
            source: Icons.arrowheadNextIcon
            opacity: tapHandler.pressed ? 0.75 : 1
            sourceSize.width: 28
            fillMode: Image.PreserveAspectFit
            color: "#929292"
        }
    }

    TapHandler {
        id: tapHandler
        onTapped: root.clicked()
    }
}
