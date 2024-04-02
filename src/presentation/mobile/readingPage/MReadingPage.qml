import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import CustomComponents
import Librum.style
import Librum.controllers
import Librum.globals

Page {
    id: root
    background: Rectangle {
        anchors.fill: parent
        color: Style.colorPageBackground
    }

    ColumnLayout {
        anchors.fill: parent

        MDocumentView {
            id: documentView
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
