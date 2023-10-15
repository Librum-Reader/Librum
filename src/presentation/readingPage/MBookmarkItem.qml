import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.controllers

Rectangle {
    id: root

    implicitHeight: 24
    color: "transparent"

    Connections {
        target: BookController

        function onStartRenamingBookmark(uuid) {
            if (uuid === model.uuid) {
                print("startRenamingBookmark")
                nameText.forceActiveFocus()
                nameText.selectAll()
            }
        }
    }

    RowLayout {
        id: layout
        anchors.fill: parent
        anchors.leftMargin: 4
        anchors.rightMargin: 4

        Label {
            id: nameText
            Layout.alignment: Qt.AlignLeft
            text: model.name
            color: Style.colorText
            font.pointSize: 11
            elide: Text.ElideRight
        }

        Label {
            id: pageNumberText
            Layout.alignment: Qt.AlignRight
            text: model.pageNumber
            color: Style.colorText
            font.pointSize: 11
        }
    }

    MouseArea {
        id: area
        anchors.fill: parent

        onClicked: BookController.goToBookmark(model.uuid)
    }
}
