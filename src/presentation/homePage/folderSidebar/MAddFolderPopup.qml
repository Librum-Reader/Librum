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
    property bool updateMode: false
    // Only needed in update mode
    property string uuid

    implicitWidth: 340
    implicitHeight: layout.height + 28
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
        nameInput.giveFocus()

        // Load the folder when in update mode
        if (root.updateMode) {
            let folder = FolderController.getFolder(uuid)

            nameInput.text = folder.name
        }
    }
    onClosed: {
        nameInput.clearText()
        updateMode = false
        uuid = ""
    }

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
                text: root.updateMode ? qsTr("Edit Folder") : qsTr(
                                            "Create Folder")
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

        MLabeledInputBox {
            id: nameInput
            Layout.fillWidth: true
            placeholderContent: qsTr("Folder Name")
            placeholderColor: Style.colorPlaceholderText
            borderWidth: 1
            inputFontSize: Fonts.size10dot5

            onReturnPressed: internal.createFolder()
        }

        Pane {
            id: moreOptions
            Layout.fillWidth: true
            Layout.preferredHeight: moreOptionsLayout.height + padding * 2
            Layout.topMargin: 14
            padding: 4
            background: Rectangle {
                anchors.fill: parent
                color: Style.colorControlBackground
                border.width: 1
                border.color: Style.colorContainerBorder
                radius: 4
            }

            ColumnLayout {
                width: parent.width
                id: moreOptionsLayout
                spacing: 3

                component IOptionButton: Item {
                    id: optionButtonRoot
                    property string optionButtonText
                    property string optionButtonIcon
                    property int iconSize

                    Layout.fillWidth: true
                    Layout.preferredHeight: 32

                    Rectangle {
                        id: optionHoverIndicator
                        anchors.fill: parent
                        color: optionButtonArea.containsMouse ? "white" : "transparent"
                        opacity: 0.08
                        radius: 4
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 8
                        anchors.rightMargin: 8
                        spacing: 0

                        Label {
                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                            text: optionButtonRoot.optionButtonText
                            color: Style.colorTitle
                            font.pointSize: Fonts.size10dot5
                        }

                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 1
                        }

                        Image {
                            Layout.alignment: Qt.AlignVCenter
                            visible: optionButtonRoot.optionButtonIcon !== ""
                            source: optionButtonRoot.optionButtonIcon
                            sourceSize.width: optionButtonRoot.iconSize
                            fillMode: Image.PreserveAspectFit
                        }

                        Image {
                            Layout.alignment: Qt.AlignVCenter
                            Layout.leftMargin: 4
                            source: Icons.arrowheadNextIcon
                            sourceSize.width: 18
                            fillMode: Image.PreserveAspectFit
                        }
                    }

                    MouseArea {
                        id: optionButtonArea
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked: {
                            console.log("clicked")
                        }
                    }
                }

                component ISeparator: Rectangle {
                    Layout.preferredWidth: parent.width * 0.94
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredHeight: 1
                    color: Style.colorSeparator
                }

                IOptionButton {
                    optionButtonText: qsTr("Icon")
                    optionButtonIcon: Icons.folder
                    iconSize: 14
                }

                ISeparator {}

                IOptionButton {
                    optionButtonText: qsTr("Color")
                    optionButtonIcon: Icons.settingsSidebarAppearance
                    iconSize: 16
                }

                ISeparator {}

                IOptionButton {
                    optionButtonText: qsTr("Description")
                    optionButtonIcon: Icons.pen
                    iconSize: 15
                }
            }
        }

        MButton {
            id: loginButton
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            Layout.topMargin: 14
            borderWidth: 0
            backgroundColor: Style.colorBasePurple
            opacity: nameInput.text.length > 0 ? 1 : 0.4
            fontSize: Fonts.size10dot5
            opacityOnPressed: 0.85
            textColor: Style.colorFocusedButtonText
            fontWeight: Font.Bold
            text: root.updateMode ? qsTr("Update") : qsTr("Create")

            onClicked: internal.createFolder()
        }
    }

    QtObject {
        id: internal

        function createFolder() {
            if (!root.updateMode) {
                FolderController.createFolder(nameInput.text)
            } else {
                FolderController.updateFolder(uuid, nameInput.text, "", "")
            }

            root.close()
        }
    }
}
