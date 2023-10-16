import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.icons
import Librum.controllers

Rectangle {
    id: root
    property string uuid: model.uuid
    signal rightClicked(var mouse, int index)

    implicitHeight: 24
    color: "transparent"

    Component.onCompleted: {
        if (bookmarksView.lastAddedUuid === model.uuid) {
            startRenaming()

            bookmarksView.lastAddedUuid = ""
        }
    }

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

            onAccepted: {
                root.forceActiveFocus()
                nameText.deselect()
                BookController.renameBookmark(model.uuid, nameText.text)

                nameText.text = Qt.binding(function () {
                    return model.name
                })
            }

            Keys.onEscapePressed: {
                nameText.deselect()
                root.forceActiveFocus()

                nameText.text = Qt.binding(function () {
                    return model.name
                })
            }
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
            text: model.pageNumber + 1
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
                           goToBookmark()
                       } else if (mouse.button === Qt.RightButton) {
                           root.rightClicked(mouse, model.index)
                       }
                   }
    }

    function goToBookmark() {
        BookController.goToBookmark(model.uuid)
    }

    function startRenaming() {
        nameText.selectAll()
        nameText.forceActiveFocus()
    }
}
