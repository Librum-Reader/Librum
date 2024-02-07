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
    signal descriptionFinished(string text)

    implicitWidth: 360
    implicitHeight: 246
    horizontalPadding: 21
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
    background: Rectangle {
        color: Style.colorPopupBackground
        border.width: 1
        border.color: Style.colorContainerBorder
        radius: 6
        antialiasing: true
    }

    onOpened: descriptionInput.forceActiveFocus()

    onClosed: {
        descriptionInput.text = ""
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 0

        RowLayout {
            Layout.preferredWidth: parent.width
            Layout.topMargin: 12

            Label {
                id: popupTitle
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                text: qsTr("Describe this folder")
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

        Rectangle {
            id: container
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 16
            color: "transparent"
            border.width: 1
            border.color: Style.colorContainerBorder
            radius: 4

            Flickable {
                anchors.fill: parent
                clip: true
                contentHeight: descriptionInput.implicitHeight
                contentWidth: parent.width
                flickDeceleration: 10000
                maximumFlickVelocity: 2000
                boundsBehavior: Flickable.StopAtBounds
                boundsMovement: Flickable.StopAtBounds
                flickableDirection: Flickable.VerticalFlick

                // Make sure its always scrolled to the bottom
                onContentHeightChanged: {
                    if (contentHeight > height) {
                        contentY = contentHeight - height
                    }
                }

                TextField {
                    id: descriptionInput
                    placeholderText: qsTr("This folder is about...")
                    placeholderTextColor: Style.colorPlaceholderText
                    width: container.width
                    wrapMode: TextEdit.Wrap
                    color: Style.colorBaseInputText
                    selectedTextColor: Style.colorBaseInputText
                    font.pointSize: Fonts.size10dot5
                    padding: 10
                    selectionColor: Style.colorTextSelection
                    background: Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                        border.width: 0
                        radius: 4
                    }

                    Keys.onReturnPressed: {
                        root.descriptionFinished(descriptionInput.text)
                        root.close()
                    }
                }
            }
        }

        MButton {
            id: doneButton
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            Layout.topMargin: 14
            Layout.bottomMargin: 6
            borderWidth: 0
            backgroundColor: Style.colorBasePurple
            fontSize: Fonts.size10dot5
            opacityOnPressed: 0.85
            textColor: Style.colorFocusedButtonText
            fontWeight: Font.Bold
            text: qsTr("Done")

            onClicked: {
                root.descriptionFinished(descriptionInput.text)
                root.close()
            }
        }
    }

    function setDescription(description) {
        internal.avoidAutoTextScroll = true
        descriptionInput.text = description
        internal.avoidAutoTextScroll = false
    }

    QtObject {
        id: internal
        property bool avoidAutoTextScroll: false
    }
}
