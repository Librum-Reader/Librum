import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.fonts
import CustomComponents

Page {
    id: root
    background: Rectangle {
        color: Style.colorPageBackground
    }

    ColumnLayout {
        id: layout
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: 64
        anchors.rightMargin: 70
        spacing: 0

        MTitle {
            id: pageTitle
            Layout.topMargin: 44
            titleText: qsTr("Tools")
            descriptionText: qsTr("Powerful tools to modify your books")
        }

        Pane {
            id: contentBlock
            Layout.fillWidth: true
            Layout.topMargin: 45
            verticalPadding: 24
            horizontalPadding: 40
            background: Rectangle {
                color: Style.colorContainerBackground
                border.color: Style.colorContainerBorder
                radius: 4
                antialiasing: true
            }

            Flow {
                id: grid
                width: parent.width
                spacing: 16

                component ToolItem: Pane {
                    id: itemRoot
                    property string text: ""
                    property string description: ""
                    signal clicked

                    implicitWidth: 380
                    implicitHeight: 220
                    opacity: area.pressed ? 0.6 : 1
                    background: Rectangle {
                        color: "#343534"
                        radius: 6
                    }

                    ColumnLayout {
                        spacing: 5
                        anchors.centerIn: parent

                        Label {
                            id: title
                            Layout.alignment: Qt.AlignHCenter
                            text: itemRoot.text
                            color: Style.colorTitle
                            font.bold: true
                            font.pointSize: Fonts.size20
                        }

                        Label {
                            id: description
                            Layout.alignment: Qt.AlignHCenter
                            text: itemRoot.description
                            color: Style.colorText
                            font.pointSize: Fonts.size13
                        }
                    }

                    MouseArea {
                        id: area
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor

                        onClicked: itemRoot.clicked()
                    }
                }

                ToolItem {
                    text: qsTr("Merge PDFs")
                    description: "Merge multiple PDFs into one"
                    onClicked: mergePdfPopup.open()
                }

                ToolItem {
                    text: qsTr("Sign PDF")
                    onClicked: mergePdfPopup.open()
                }

                ToolItem {
                    text: qsTr("Sign PDF")
                    onClicked: mergePdfPopup.open()
                }

                ToolItem {
                    text: qsTr("Sign PDF")
                    onClicked: mergePdfPopup.open()
                }

                ToolItem {
                    text: qsTr("Sign PDF")
                    onClicked: mergePdfPopup.open()
                }

                ToolItem {
                    text: qsTr("Sign PDF")
                    onClicked: mergePdfPopup.open()
                }
            }
        }
    }

    MMergePdfToolPopup {
        id: mergePdfPopup
        width: parent.width * 0.6
        height: parent.height * 0.65
        y: (parent.height - height) / 2
        x: (parent.width - width) / 2
    }
}
