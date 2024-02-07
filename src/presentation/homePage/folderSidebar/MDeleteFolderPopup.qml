import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.fonts
import Librum.style
import Librum.icons
import Librum.controllers
import CustomComponents

Popup {
    id: root
    property string uuid

    implicitWidth: 420
    implicitHeight: layout.height + 32
    horizontalPadding: 20
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
    background: Rectangle {
        color: Style.colorPopupBackground
        border.width: 1
        border.color: Style.colorContainerBorder
        radius: 6
        antialiasing: true
    }

    onOpened: {
        root.forceActiveFocus()

        let folder = FolderController.getFolder(uuid)
        internal.folderName = folder.name
    }
    onClosed: uuid = ""

    ColumnLayout {
        id: layout
        width: parent.width
        spacing: 0

        RowLayout {
            Layout.preferredWidth: parent.width
            Layout.topMargin: 12

            Label {
                id: popupTitle
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                text: qsTr("Delete ") + internal.folderName
                elide: Text.ElideRight
                font.weight: Font.Medium
                font.pointSize: Fonts.size16
                color: Style.colorTitle
            }

            MButton {
                id: closeButton
                Layout.preferredHeight: 32
                Layout.preferredWidth: 32
                Layout.alignment: Qt.AlignRight
                backgroundColor: "transparent"
                opacityOnPressed: 0.7
                borderColor: "transparent"
                radius: 6
                borderColorOnPressed: Style.colorButtonBorder
                imagePath: Icons.closePopup
                imageSize: 14

                onClicked: root.close()
            }
        }

        Label {
            id: infoText
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 22
            font.pointSize: Fonts.size10dot75
            color: Style.colorText
            text: qsTr("Books inside the folder will not be deleted.")
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: 22
            height: cancelButton.height
            spacing: 12

            MButton {
                id: cancelButton
                Layout.fillWidth: true
                Layout.preferredHeight: 34
                borderWidth: 1
                backgroundColor: "transparent"
                fontSize: Fonts.size10dot5
                opacityOnPressed: 0.85
                textColor: Style.colorText
                fontWeight: Font.Medium
                text: qsTr("Cancel")

                onClicked: root.close()
            }

            MButton {
                id: deleteButton
                Layout.fillWidth: true
                Layout.preferredHeight: 34
                borderWidth: 0
                backgroundColor: Style.colorRed
                fontSize: Fonts.size10dot5
                opacityOnPressed: 0.85
                textColor: Style.colorFocusedButtonText
                fontWeight: Font.Medium
                text: qsTr("Delete")

                onClicked: {
                    FolderController.deleteFolder(uuid)
                    LibraryController.removeAllBooksFromFolderWithId(uuid)
                    root.close()
                }
            }
        }
    }

    QtObject {
        id: internal
        property string folderName
    }
}
