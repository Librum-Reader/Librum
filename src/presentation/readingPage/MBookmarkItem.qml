import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.icons
import Librum.controllers

Rectangle {
    id: root
    signal rightClicked(var mouse, int index)

    implicitHeight: 24
    color: "transparent"

    RowLayout {
        id: layout
        anchors.fill: parent

        Image {
            id: imageToLeft
            Layout.alignment: Qt.AlignLeft
            Layout.leftMargin: 6
            source: Icons.readingViewBookmark
            sourceSize.width: 12
            fillMode: Image.PreserveAspectFit
        }

        TextInput {
            id: nameText
            Layout.alignment: Qt.AlignLeft
            Layout.leftMargin: 4
            text: model.name
            color: Style.colorText
            font.pointSize: 11
            selectionColor: Style.colorTextSelection
        }

        Item {
            id: widthFiller
            Layout.preferredHeight: 1
            Layout.fillWidth: true
        }

        Label {
            id: pageNumberText
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 6
            text: model.pageNumber
            color: Style.colorText
            font.pointSize: 11
        }
    }

    MouseArea {
        id: area
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: mouse => {
                       if (mouse.button === Qt.LeftButton) {
                           BookController.goToBookmark(model.uuid)
                       } else if (mouse.button === Qt.RightButton) {

                           root.rightClicked(mouse, model.index)
                       }
                   }
    }
}
