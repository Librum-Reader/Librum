import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.controllers
import Librum.fonts

Popup {
    id: root
    padding: 32
    bottomPadding: 42
    topPadding: 42
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    background: Rectangle {
        color: Style.colorPopupBackground
        radius: 6
        border.width: 1
        border.color: Style.colorContainerBorder
    }

    MFlickWrapper {
        id: flickWrapper
        anchors.fill: parent
        contentHeight: layout.height

        ColumnLayout {
            width: parent.width
            spacing: 80

            RowLayout {
                id: layout
                width: parent.width
                spacing: 0

                Label {
                    id: popupTitle
                    Layout.leftMargin: 36
                    text: qsTr("Merge PDFs")
                    font.weight: Font.Bold
                    font.pointSize: Fonts.size20
                    color: Style.colorTitle
                }

                Item {
                    Layout.fillWidth: true
                }

                MButton {
                    id: closeButton
                    Layout.preferredHeight: 36
                    Layout.preferredWidth: 36
                    Layout.rightMargin: 18
                    Layout.alignment: Qt.AlignRight
                    backgroundColor: "transparent"
                    opacityOnPressed: 0.7
                    borderColor: "transparent"
                    radius: 6
                    borderColorOnPressed: Style.colorButtonBorder
                    imagePath: Icons.closePopup
                    imageSize: 16

                    onClicked: root.close()
                }
            }

            MBookSelector {
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }
}
