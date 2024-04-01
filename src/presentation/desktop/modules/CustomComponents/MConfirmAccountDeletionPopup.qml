import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import CustomComponents
import Librum.style
import Librum.icons
import Librum.fonts
import Librum.controllers
import Librum.globals

Popup {
    id: root
    signal deletionConfirmed

    implicitWidth: 751
    implicitHeight: layout.implicitHeight
    focus: true
    padding: 0
    background: Rectangle {
        radius: 6
        color: Style.colorPopupBackground
    }
    modal: true
    Overlay.modal: Rectangle {
        color: Style.colorPopupDim
        opacity: 1
    }

    onOpenedChanged: if (!opened)
                         emailInput.text = ""

    MFlickWrapper {
        id: flickWrapper
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
                Layout.rightMargin: 14
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

            Label {
                id: popupTitle
                Layout.topMargin: 20
                Layout.leftMargin: 52
                text: qsTr("Confirm Account Deletion")
                font.weight: Font.Bold
                font.pointSize: Fonts.size19
                color: Style.colorTitle
            }

            Label {
                id: infoText
                Layout.topMargin: 20
                Layout.leftMargin: baseRoot.rightAlign ? 0 : 52
                Layout.rightMargin: baseRoot.rightAlign ? 52 : 0
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                text: qsTr("Deleting your account is an irreversible action.<br>Once you delete your account, there is <b>no</b> going back. Please be certain.")
                font.pointSize: Fonts.size14
                color: Style.colorText
            }

            MLabeledInputBox {
                id: emailInput
                Layout.fillWidth: true
                Layout.topMargin: 28
                Layout.leftMargin: 52
                Layout.rightMargin: 52
                headerFontSize: Fonts.size11
                placeholderContent: qsTr("Your Email")
                placeholderColor: Style.colorPlaceholderText
                headerText: qsTr("Confirm the deletion by entering your Account's email.")
            }

            RowLayout {
                id: buttonLayout
                Layout.topMargin: 72
                Layout.bottomMargin: 42
                Layout.leftMargin: 52
                spacing: 16

                MButton {
                    id: cancelButton
                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignLeft
                    active: true
                    borderWidth: active ? 0 : 1
                    backgroundColor: active ? Style.colorBasePurple : "transparent"
                    opacityOnPressed: 0.7
                    text: qsTr("Cancel")
                    textColor: active ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                    fontWeight: Font.Bold
                    fontSize: Fonts.size12

                    onClicked: root.close()
                }

                MButton {
                    id: deleteButton
                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignRight
                    borderWidth: active ? 0 : 1
                    backgroundColor: active ? Style.colorRed : "transparent"
                    opacityOnPressed: 0.7
                    text: qsTr("Delete")
                    textColor: active ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                    fontWeight: Font.Bold
                    fontSize: Fonts.size12
                    imagePath: active ? Icons.trashHighlighted : Icons.trash
                    imageSize: 17
                    imageSpacing: 10

                    onClicked: {
                        if (emailInput.text !== UserController.email) {
                            emailInput.errorText = qsTr("Your email is wrong")
                            emailInput.setError()
                            return
                        }

                        root.deletionConfirmed()
                        root.close()
                    }
                }
            }
        }
    }
}
