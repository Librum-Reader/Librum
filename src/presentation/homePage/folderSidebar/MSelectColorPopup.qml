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
    signal colorSelected(string color)

    implicitWidth: 340
    implicitHeight: layout.height + 32
    horizontalPadding: 21
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
    background: Rectangle {
        color: Style.colorPopupBackground
        border.width: 1
        border.color: Style.colorContainerBorder
        radius: 6
        antialiasing: true
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
                Layout.fillWidth: true
                text: qsTr("Pick a color")
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

        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 24
            Layout.bottomMargin: 8
            Layout.leftMargin: 1
            columnSpacing: 12
            rowSpacing: 12
            columns: 7

            component ColorRect: Rectangle {
                id: colorRect
                Layout.preferredWidth: 32
                Layout.preferredHeight: 32
                opacity: colorRectArea.containsMouse ? 0.7 : 1
                radius: 6
                border.width: 0
                antialiasing: true

                MouseArea {
                    id: colorRectArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        if (colorRect.color === Style.colorDefaultFolderIcon)
                            root.colorSelected("default")
                        else
                            root.colorSelected(colorRect.color)
                        root.close()
                    }
                }
            }

            ColorRect {
                color: Style.colorDefaultFolderIcon
            }

            ColorRect {
                color: "#1B992D"
            }

            ColorRect {
                color: "#A4FF00"
            }

            ColorRect {
                color: "#946BDE"
            }

            ColorRect {
                color: "#FF74CB"
            }

            ColorRect {
                color: "#FFD60A"
            }

            ColorRect {
                color: "#FF9F0A"
            }

            ColorRect {
                color: "#DD2D33"
            }

            ColorRect {
                color: "#FF453A"
            }

            ColorRect {
                color: "#FF375F"
            }

            ColorRect {
                color: "#0A84FF"
            }

            ColorRect {
                color: "#5E5CE6"
            }

            ColorRect {
                color: "#218383"
            }

            ColorRect {
                color: "#00F0E4"
            }
        }
    }

    QtObject {
        id: internal
        property string folderName
    }
}
