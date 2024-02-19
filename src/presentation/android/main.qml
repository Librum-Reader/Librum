import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    visibility: Window.Maximized
    title: qsTr("Librum")
    background: Rectangle {
        anchors.fill: parent
        color: "red"
    }
}
