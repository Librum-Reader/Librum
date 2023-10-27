import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.controllers
import Librum.style
import Librum.icons
import Librum.elements
import Librum.fonts

Popup {
    id: windowsUpdatingPopup
    implicitWidth: 560
    implicitHeight: layout.height
    padding: 0
    horizontalPadding: 52
    background: Rectangle {
        color: Style.colorPopupBackground
        radius: 6
    }
    modal: true
    Overlay.modal: Rectangle {
        color: Style.colorPopupDim
        opacity: 1
    }

    onClosed: progressBarFill.width = 0

    MFlickWrapper {
        anchors.fill: parent
        contentHeight: layout.height

        ColumnLayout {
            id: layout
            width: parent.width
            spacing: 0

            MButton {
                id: closeButton
                Layout.preferredHeight: 32
                Layout.preferredWidth: 32
                Layout.topMargin: 12
                Layout.rightMargin: -38
                Layout.alignment: Qt.AlignTop | Qt.AlignRight
                backgroundColor: "transparent"
                opacityOnPressed: 0.7
                borderColor: "transparent"
                radius: 6
                borderColorOnPressed: Style.colorButtonBorder
                imagePath: Icons.closePopup
                imageSize: 14

                onClicked: windowsUpdatingPopup.close()
            }

            Label {
                id: popupTitle
                Layout.alignment: Qt.AlignHCenter
                Layout.bottomMargin: 100
                text: "Updating"
                font.weight: Font.Bold
                font.pointSize: Fonts.size22
                color: Style.colorBasePurple
            }

            Label {
                text: "Downloading..."
                font.pointSize: Fonts.size14
                color: Style.colorText
            }

            Rectangle {
                id: progressBar
                Layout.fillWidth: true
                Layout.preferredHeight: 24
                Layout.topMargin: 4
                Layout.bottomMargin: 80

                Rectangle {
                    id: progressBarFill
                    width: 0
                    height: parent.height
                    color: Style.colorBasePurple
                }
            }
        }
    }

    function setDownloadProgress(progress) {
        progressBarFill.width = progressBar.width * progress
    }
}
