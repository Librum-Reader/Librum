import QtQuick
import QtQuick.Layouts
import Librum.style

Item {
    id: root
    implicitWidth: 72
    implicitHeight: 72

    Rectangle {
        id: outerRect
        anchors.fill: parent
        color: Style.colorBasePurple
        radius: 4
        antialiasing: true

        Rectangle {
            id: innerRect
            width: parent.width / 2
            height: parent.height / 2
            anchors.centerIn: parent
            color: Style.colorContainerBackground
            radius: width / 7
            antialiasing: true
        }
    }
}
