import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.controllers
import Librum.style
import Librum.icons
import Librum.fonts

Popup {
    id: root
    signal settingsChanged

    implicitWidth: 190
    closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    padding: 0
    background: Rectangle {
        color: "transparent"
    }
    clip: true

    ColumnLayout {
        id: mainLayout
        width: parent.width
        spacing: 0

        Pane {
            id: container
            Layout.fillWidth: true
            verticalPadding: 15
            horizontalPadding: 12
            background: Rectangle {
                color: Style.colorPopupBackground
                border.color: Style.colorContainerBorder
                border.width: 1
                radius: 4
            }

            ColumnLayout {
                id: itemLayout
                anchors.fill: parent
                spacing: 12

                MLabeledCheckBox {
                    id: fromStartBox
                    Layout.fillWidth: true
                    boxWidth: 18
                    boxHeight: 18
                    spacing: 8
                    imageSize: 10
                    checked: BookController.searchFromStart
                    text: qsTr("From start")
                    fontSize: Fonts.size12

                    onCheckedChanged: internal.updateSearchOptions()
                }

                MLabeledCheckBox {
                    id: caseSensitiveBox
                    Layout.fillWidth: true
                    boxWidth: 18
                    boxHeight: 18
                    spacing: 8
                    imageSize: 10
                    checked: BookController.searchCaseSensitive
                    text: qsTr("Case sensitive")
                    fontSize: Fonts.size12

                    onCheckedChanged: internal.updateSearchOptions()
                }

                MLabeledCheckBox {
                    id: wholeWordsBox
                    Layout.fillWidth: true
                    boxWidth: 18
                    boxHeight: 18
                    spacing: 8
                    imageSize: 10
                    checked: BookController.searchWholeWords
                    text: qsTr("Whole words")
                    fontSize: Fonts.size12

                    onCheckedChanged: internal.updateSearchOptions()
                }
            }
        }

        Image {
            id: dropletIcon
            Layout.leftMargin: 13
            Layout.topMargin: -1
            source: Icons.popupDroplet
            sourceSize.width: 10
            fillMode: Image.PreserveAspectFit
            rotation: 180
        }
    }

    QtObject {
        id: internal

        function updateSearchOptions() {
            BookController.searchWholeWords = wholeWordsBox.checked
            BookController.searchCaseSensitive = caseSensitiveBox.checked
            BookController.searchFromStart = fromStartBox.checked
            root.settingsChanged()
        }
    }
}
