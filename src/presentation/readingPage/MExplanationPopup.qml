import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.controllers

Popup {
    id: root
    property string answer
    property string question

    implicitWidth: 800
    implicitHeight: 700
    padding: 32
    bottomPadding: 28
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    background: Rectangle {
        color: Style.colorPopupBackground
        radius: 6
        border.width: 1
        border.color: Style.colorContainerBorder
    }

    onClosed: {
        root.answer = ""
        root.question = ""
    }

    ColumnLayout {
        anchors.fill: parent

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

        MComboBox {
            id: modesComboBox
            Layout.preferredWidth: 300
            Layout.preferredHeight: 60
            itemHeight: 32
            headerText: "Mode"
            headerFontSize: 12
            headerFontColor: Style.colorTitle
            headerFontWeight: Font.DemiBold
            fontSize: 12
            selectedItemFontColor: Style.colorReadOnlyInputText
            selectedItemFontSize: 13
            selectedItemPadding: 3
            defaultIndex: 0
            emptyText: "None selected"
            dropdownIconSize: 11
            contentPropertyName: "name"

            checkBoxStyle: false
            model: ListModel {
                ListElement {
                    name: "Default"
                }
                ListElement {
                    name: "Explain like I'm five"
                }
                ListElement {
                    name: "In-Depth"
                }
                ListElement {
                    name: "Summarize"
                }
                ListElement {
                    name: "Explain visually"
                }
            }

            onItemChanged: modesComboBox.closePopup()
        }

        MLabeledInputBox {
            id: request
            Layout.fillWidth: true
            Layout.topMargin: 24
            headerFontColor: Style.colorLightText
            headerFontSize: 12
            borderColor: Style.colorContainerBorder
            borderWidth: 1
            text: question
            backgroundColor: Style.colorContainerBackground
            inputFontSize: 13
            headerText: "Request"
        }

        Pane {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 32
            background: Rectangle {
                color: Style.colorContainerBackground
                radius: 6
                border.width: 1
                border.color: Style.colorContainerBorder
            }

            TextEdit {
                id: content
                anchors.fill: parent
                font.pointSize: 13
                text: root.answer
                readOnly: true
                wrapMode: Text.WordWrap
                color: Style.colorText
            }
        }
    }
}
