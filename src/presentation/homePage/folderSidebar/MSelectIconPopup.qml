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
    signal iconSelected(string name)

    implicitWidth: 337
    implicitHeight: 600
    horizontalPadding: 21
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
    background: Rectangle {
        color: Style.colorPopupBackground
        border.width: 1
        border.color: Style.colorContainerBorder
        radius: 6
        antialiasing: true
    }

    onOpened: searchField.forceActiveFocus()
    onClosed: searchField.text = ""

    ColumnLayout {
        id: layout
        width: parent.width
        height: parent.height
        spacing: 0

        RowLayout {
            Layout.preferredWidth: parent.width
            Layout.topMargin: 12

            Label {
                id: popupTitle
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                text: qsTr("Pick an icon")
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

        Pane {
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            Layout.topMargin: 18
            padding: 0

            background: Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.width: 1
                border.color: Style.colorContainerBorder
                radius: 4
            }

            LayoutMirroring.enabled: baseRoot.rightAlign
            LayoutMirroring.childrenInherit: true

            RowLayout {
                anchors.fill: parent
                spacing: 1

                Image {
                    Layout.leftMargin: 12
                    Layout.preferredWidth: implicitWidth
                    Layout.alignment: Qt.AlignVCenter
                    source: Icons.search
                    fillMode: Image.PreserveAspectFit
                    sourceSize.height: 12
                }

                TextField {
                    id: searchField
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    placeholderText: qsTr("Search")
                    color: Style.colorBaseInputText
                    font.pointSize: Fonts.size10dot5
                    placeholderTextColor: Style.colorPlaceholderText
                    background: Rectangle {
                        color: "transparent"
                    }

                    onTextChanged: {
                        FolderController.iconModel.sortString = text
                    }
                }
            }
        }

        GridView {
            id: view
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 10
            Layout.bottomMargin: 8
            Layout.leftMargin: 1
            flickDeceleration: 10000
            maximumFlickVelocity: 2000
            boundsBehavior: Flickable.StopAtBounds
            boundsMovement: Flickable.StopAtBounds
            flickableDirection: Flickable.VerticalFlick
            clip: true
            cellWidth: 36 + 7
            cellHeight: 36 + 7
            rightMargin: -10

            model: FolderController.iconModel
            delegate: Rectangle {
                id: iconRect
                required property string name

                width: 36
                height: 36
                opacity: iconRectArea.containsMouse ? 0.7 : 1
                radius: 6
                border.width: 1
                border.color: Style.colorContainerBorder
                color: "transparent"

                Image {
                    id: iconImage
                    anchors.centerIn: parent
                    sourceSize.width: 26
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/resources/images/folder_icons/" + iconRect.name + ".svg"
                }

                MouseArea {
                    id: iconRectArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        root.iconSelected(iconRect.name)
                        root.close()
                    }
                }
            }
        }
    }

    QtObject {
        id: internal
        property string folderName
    }
}
