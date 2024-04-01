import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.icons

Item {
    id: root
    signal clicked

    implicitWidth: 40
    implicitHeight: 40
    opacity: tapHandler.pressed ? 0.07 : 1

    Rectangle {
        anchors.fill: parent
        color: Style.colorTransparentHighlight
        opacity: tapHandler.pressed ? 0.5 : 0
        radius: 6
    }

    IconImage {
        id: backArrowIcon
        anchors.centerIn: parent
        source: Icons.mobileArrowLeft
        sourceSize.width: 24
        color: "#121212"
    }

    TapHandler {
        id: tapHandler

        onTapped: root.clicked()
    }
}
