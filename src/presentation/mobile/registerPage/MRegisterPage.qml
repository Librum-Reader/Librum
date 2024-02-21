import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Librum.style
import Librum.fonts

Page {
    id: page
    horizontalPadding: 18
    background: Rectangle {
        color: Style.colorPageBackground
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
    }
}
