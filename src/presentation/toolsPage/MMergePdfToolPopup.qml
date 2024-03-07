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

    Component.onCompleted: {
        LibraryController.bookTitleModel.showOnlyDownloaded = true
        LibraryController.bookTitleModel.format = "pdf"
    }

    Component.onDestruction: {
        LibraryController.bookTitleModel.showOnlyDownloaded = false
        LibraryController.bookTitleModel.format = ""

        bookSelector.selectedItems = []
    }

    MFlickWrapper {
        id: flickWrapper
        anchors.fill: parent
        contentHeight: layout.height

        ColumnLayout {
            width: parent.width
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

            MBookSelector {
                id: bookSelector
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 80

                onCountChanged: {
                    listView.model = 0
                    listView.model = bookSelector.selectedItems.length
                }
            }

            ListView {
                id: listView
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                Layout.maximumHeight: 400
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 16
                Layout.bottomMargin: 20
                model: 0
                delegate: Rectangle {
                    id: delRoot
                    property string title: bookSelector.selectedItems[modelData].title
                    property string uuid: bookSelector.selectedItems[modelData].uuid

                    radius: 4
                    color: "#222222"
                    border.width: 1
                    border.color: "#1E1E1E"
                    height: 36
                    width: parent.width

                    RowLayout {
                        anchors.fill: parent

                        Label {
                            Layout.topMargin: 4
                            Layout.fillWidth: true
                            Layout.leftMargin: 8
                            Layout.alignment: Qt.AlignVCenter
                            text: delRoot.title
                            color: Style.colorText
                            font.pointSize: Fonts.size14
                        }

                        MButton {
                            Layout.preferredHeight: 34
                            Layout.preferredWidth: 28
                            Layout.rightMargin: 8
                            backgroundColor: Style.colorContainerBackground
                            opacityOnPressed: 0.7
                            borderColor: "transparent"
                            radius: 6
                            borderColorOnPressed: Style.colorButtonBorder
                            imagePath: Icons.arrowheadNextIcon
                            imageRotation: -90
                            imageSize: 18

                            onClicked: bookSelector.moveBookUp(delRoot.uuid)
                        }

                        MButton {
                            Layout.preferredHeight: 34
                            Layout.preferredWidth: 28
                            Layout.rightMargin: 4
                            backgroundColor: Style.colorContainerBackground
                            opacityOnPressed: 0.7
                            borderColor: "transparent"
                            radius: 6
                            borderColorOnPressed: Style.colorButtonBorder
                            imagePath: Icons.arrowheadNextIcon
                            imageRotation: 90
                            imageSize: 18

                            onClicked: bookSelector.moveBookDown(delRoot.uuid)
                        }

                        MButton {
                            Layout.preferredHeight: 34
                            Layout.preferredWidth: 28
                            Layout.rightMargin: 4
                            backgroundColor: Style.colorRed
                            opacityOnPressed: 0.7
                            borderColor: "transparent"
                            radius: 6
                            borderColorOnPressed: Style.colorButtonBorder
                            imagePath: Icons.closePopupWhite
                            imageSize: 14

                            onClicked: bookSelector.removeBookFromSelection(
                                           delRoot.uuid)
                        }
                    }
                }
            }
        }
    }
}
