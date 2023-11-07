import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.fonts

Page {
    id: root
    background: Rectangle {
        color: Style.colorPageBackground
    }

    Item {
        id: topSpacer
        height: parent.height / 2.3
    }

    Label {
        id: title
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: topSpacer.bottom
        text: qsTr("Statistics Page")
        color: Style.colorTitle
        font.pointSize: Fonts.size22
        font.bold: true
    }

    Label {
        id: description
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: title.bottom
        anchors.topMargin: 6
        text: qsTr("Currently in Development")
        color: Style.colorPageSubtitle
        font.pointSize: Fonts.size16
        font.bold: true
    }
}
