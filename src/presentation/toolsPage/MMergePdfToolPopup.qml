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
    implicitWidth: 400
    implicitHeight: 300
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
            spacing: 0

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

            RowLayout {
                id: inputLayout
                spacing: 24
                Layout.topMargin: 240

                MLabeledInputBox {
                    id: firstInput
                    Layout.preferredWidth: 200
                    placeholderContent: "Some Book"
                    placeholderColor: Style.colorPlaceholderText
                    headerText: qsTr("First")
                }

                MLabeledInputBox {
                    id: secondInput
                    Layout.preferredWidth: 200
                    placeholderContent: "Another Book"
                    placeholderColor: Style.colorPlaceholderText
                    headerText: qsTr("Second")
                }

                MButton {
                    id: loginButton
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 40
                    borderWidth: 0
                    backgroundColor: Style.colorBasePurple
                    fontSize: Fonts.size12
                    opacityOnPressed: 0.85
                    textColor: Style.colorFocusedButtonText
                    fontWeight: Font.Bold
                    text: qsTr("Merge")

                    onClicked: ToolsController.mergePdfs(firstInput.text,
                                                         secondInput.text)
                }
            }
        }
    }
}
